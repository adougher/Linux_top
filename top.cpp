/**
 * Contains the main() routine of what will eventually be your version of top.
 */

#include "sysinfo.h"
#include <cstdlib>
#include <ncurses.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include "types/cpu.h"
#include "types/load_average.h"
#include "types/memory_info.h"
#include "types/process.h"
#include "types/system_info.h"
#include "top_functions.h"

using namespace std;

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}

/**
 * Entry point for the program.
 */
int main( int argc, char** argv ) {

  float delay_time = 1.0;
  int do_sort = -1, help = -1;
  string sort_opt="";
  // populate options or exit if invalid
  if( argc > 1 )
  {
    topf::get_options( delay_time, do_sort, help, sort_opt, argc, argv );
  }

  // ncurses initialization
  initscr();

  // Don't show a cursor.
  curs_set(FALSE);

  // Set getch to return after delay_time*1000 milliseconds; this allows the 
  // program to immediately respond to user input while not blocking indefinitely.
  timeout(delay_time*1000);

  SystemInfo sys_info=get_system_info(); // Collect system info from /proc
  SystemInfo sys_info_prev;
  while (true) {
    wclear(stdscr);

    // get current time
    string stime=topf::get_time();  

    // get system uptime in proper format
    int days=floor(sys_info.uptime/(3600.0*24));
    int hours=floor((sys_info.uptime/(3600.0*24)-days)*24);
    int mins=floor(((sys_info.uptime/(3600.0*24)-days)*24-hours)*60.0);

    // Get memory info
    MemoryInfo meminfo=get_memory_info();

    // Keep previous sys_info for calculations involving a diff
    sys_info_prev=sys_info;
    sys_info=get_system_info();

    // Get number of users
    int num_users=topf::get_num_users( sys_info );

    // gather info on CPU percentages
    double individual_cpu_percentages[3*sys_info.cpus.size( )];
    unsigned long long ttime;
    unsigned long long ttime_prev;
    topf::get_cpu_percent( sys_info, sys_info_prev, ttime, ttime_prev, individual_cpu_percentages );

    // Format everything nicely given all cases
    if( days == 1 )
    {
      printw("top - %s up %d day,  %02d:%02d,  %d users,  load average: %.2f, %.2f, %.2f\n",stime.c_str( ),days,
              hours,mins,num_users,sys_info.load_average.one_min,sys_info.load_average.five_mins,
              sys_info.load_average.fifteen_mins);
    }
    else
    {
      printw("top - %s up %d days,  %02d:%02d,  %d users,  load average: %.2f, %.2f, %.2f\n",stime.c_str( ),days,
              hours,mins,num_users,sys_info.load_average.one_min,sys_info.load_average.five_mins,
              sys_info.load_average.fifteen_mins);
    }

    // Print task info
    printw("Tasks: %d total,  %d running\nThreads: %d total   User Threads: %d total   Kernel Threads: %d total\n",
            sys_info.processes.size( ),sys_info.num_running, sys_info.num_threads, sys_info.num_user_threads, 
            sys_info.num_kernel_threads );

    // Print CPU info
    printw("%10s %6.1f%s %6.1f%s %6.1f%s\n","Cpu(s):",individual_cpu_percentages[0],"%us,",individual_cpu_percentages[1],
           "%sy,",individual_cpu_percentages[2],"%id");
    for( size_t i=1; i<sys_info.cpus.size( ); i++ )
    {
      if( i%2==1 && i != sys_info.cpus.size( )-1)
      {
        printw("%7s %d%c %6.1f%s %6.1f%s %6.1f%s","Cpu",i,':',individual_cpu_percentages[3*i],"%us,",
                individual_cpu_percentages[3*i+1],"%sy,",individual_cpu_percentages[3*i+2],"%id");
      }
      else
      {
        printw("%7s %d%c %6.1f%s %6.1f%s %6.1f%s\n","Cpu",i,':',individual_cpu_percentages[3*i],"%us,",
                individual_cpu_percentages[3*i+1],"%sy,",individual_cpu_percentages[3*i+2],"%id");
      }
    }

    // Print memory info
    printw("%-5s %9d%s %8d%s %8d%s %8d%s\n","Mem:",meminfo.total_memory,
            "k total,  ",meminfo.total_memory-meminfo.free_memory,"k used,  ",meminfo.free_memory,
            "k free,  ",meminfo.buffers_memory,"k buffers");
    printw("%-5s %9d%s %8d%s %8d%s %8d%s\n\n","Swap:",meminfo.total_swap,
            "k total,  ",meminfo.total_swap-meminfo.free_swap,"k used,  ",meminfo.free_swap,
            "k free,  ",meminfo.cached_memory,"k buffers");

    // Print process info
    attron(A_STANDOUT);
    printw("%9s %13s %9s %7s %7s %14s %20s\n","PID","USER","RES","STATE","%CPU","TIME+ ","COMMAND");
    attroff(A_STANDOUT);

    vector<topf::Proc_and_More> procs(sys_info.processes.size( ));
    topf::fill_procs(procs,sys_info.processes.size( ),sys_info,sys_info_prev,ttime,ttime_prev);
    if( do_sort != -1 )
    {
      topf::sort_procs( procs, sort_opt );
    }

    string sproc_time;
    int procs_to_show=30;
    for( int i=0; i<procs_to_show; i++ )
    {
      sproc_time=topf::get_proc_time( procs[i],mins,hours );
      string scomm=procs[i].p.comm;
      scomm.erase( 0,1 );
      scomm.erase( scomm.size( )-1,1 );
      printw("%9d %13s %9s %7c %7.1f %14s %20s\n",procs[i].p.pid,procs[i].p.user_name.c_str( ),
              topf::get_pages_bytes( procs[i].p.rss ).c_str( ),procs[i].p.state,procs[i].percent_cpu,
              sproc_time.c_str( ),scomm.c_str( ));
    }

    // Redraw the screen.
    refresh();
    
    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}
