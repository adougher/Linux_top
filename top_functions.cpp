#include <vector>
#include <string>
#include <sstream>
#include <getopt.h>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <set>
#include "types/process.h"
#include "top_functions.h"

using namespace std;

// These functions are used to sort the Proc_and_More data structure
// on the given fields.
bool PIDcomp( const topf::Proc_and_More& p1, const topf::Proc_and_More& p2 )
{
    return p1.p.pid < p2.p.pid;
}

bool MEMcomp( const topf::Proc_and_More& p1, const topf::Proc_and_More& p2 )
{
    return p1.p.rss > p2.p.rss;
}

bool CPUcomp( const topf::Proc_and_More& p1, const topf::Proc_and_More& p2 )
{
    return p1.percent_cpu > p2.percent_cpu;
}

bool TIMEcomp( const topf::Proc_and_More& p1, const topf::Proc_and_More& p2 )
{
    return p1.proc_time > p2.proc_time;
}

void print_usage( )
{
  cout << "./mytop: Usage: ./mytop [-h -d delay -s sort-type] | [--help --sort-key sort-type --delay delay]\n";
  cout << "  '-h' and '--help' print this message.\n";
  cout << "  '-s sort-type' and '--sort-key sort-type' sort the output according to the sort-type fields:\n";
  cout << "    -CPU: sorts processes from highest to lowest CPU usage.\n";
  cout << "    -MEM: sorts processes from highest to lowest resident set size.\n";
  cout << "    -PID: sorts processes from lowest to highest pid.\n";
  cout << "    -TIME: sorts processes from highest to lowest cumulative time processed.\n";
  cout << "  '-d delay' and '--delay delay' set the refresh rate in 10ths of seconds.\n";
  cout << "   The minimum value is 0.1 qnd the maximum in 1000.0.\n"; 
}

int topf::index_find( vector<Process>& procs, int curpid )
{
  int index=-1;
  for( size_t i=0; i<procs.size( ); i++ )
  {
    if( procs[i].pid==curpid )
    {
      index=i;
      break;
    }
  }
  return index;
}

string topf::get_pages_bytes( long rss )
{
    rss*=sysconf(_SC_PAGESIZE);
    double rssf=double(rss);
    int index=0;
    while( rssf > 1024 )
    {
      rssf/=1024.0;
      index++;
    }
    string srss;
    ostringstream temp;
    if( rssf < 10 )
    {
      rssf*=1024.0;
      rss=long(rssf);
      temp << rss;
      srss=temp.str( );
    }
    else
    {
      rss=long(rssf);
      temp << rss;
      srss=temp.str( );
      if( index==2 )
      {
        srss=srss + "m";
      }
      if( index==3 )
      {
        srss=srss + "g";
      }
    }
    return srss;
}

void topf::get_options( float& delay_time, int& do_sort, int& help, string& sort_opt, int argc, char** argv )
{
    int opt= 0;
    //Specifying the expected options
    //The options d, s and h expect numbers as argument
    static struct option long_options[] = {
        {"delay",     required_argument, 0,  'd' },
        {"sort-key",  required_argument, 0,  's' },
        {"help",      no_argument,       0,  'h' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"d:s:h",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'd' : delay_time = atof(optarg);
                 break;
             case 's' : do_sort = 0; sort_opt=string(optarg);
                 break;
             case 'h' : help = 0;
                 break;
             default: print_usage();
                 exit(EXIT_FAILURE);
        }
    }
    if( help==0 )
    {
      print_usage( );
      exit(EXIT_SUCCESS);
    }
    if (delay_time < 0.1 || delay_time > 1000.0 ) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    if( do_sort != -1 )
    {
      if ( sort_opt != "PID" && sort_opt != "CPU" && sort_opt != "MEM" && sort_opt != "TIME" ) {
          print_usage();
          exit(EXIT_FAILURE);
      }
    }
}

string topf::get_time( )
{
    time_t raw_time;
    time(&raw_time);
    char * rtime=ctime(&raw_time);
    string stime(rtime);
    stime.erase(0,stime.find_first_of("123456789"));
    stime.erase(0,stime.find_first_of(" ")+1);
    stime.erase(stime.find_first_of(" "),100);
    return stime;
}

int topf::get_num_users( SystemInfo& sys_info )
{
  set<string> users;
  for( size_t i=0; i<sys_info.processes.size( ); i++ )
  {
    users.insert( sys_info.processes[i].user_name );
  }
  int num_users=users.size( );
  return num_users;
}

void topf::get_cpu_percent( SystemInfo& sys_info,SystemInfo& sys_info_prev,unsigned long long& ttime,unsigned long long& ttime_prev,
                      double* individual_cpu_percentages)
{
  ttime=sys_info.cpus[0].total_time();
  ttime_prev=sys_info_prev.cpus[0].total_time();
  for( size_t i=0; i<sys_info.cpus.size( ); i++ )
    {
      individual_cpu_percentages[3*i]=round(1000.0*(sys_info.cpus[i].user_time-sys_info_prev.cpus[i].user_time)/
                                      (sys_info.cpus[i].total_time( )-sys_info_prev.cpus[i].total_time( )) )/10.0;
      individual_cpu_percentages[3*i+1]=round(1000.0*(sys_info.cpus[i].total_system_time()-sys_info_prev.cpus[i].total_system_time())/
                                        (sys_info.cpus[i].total_time( )-sys_info_prev.cpus[i].total_time( )) )/10.0;
      individual_cpu_percentages[3*i+2]=round(1000.0*(sys_info.cpus[i].total_idle_time()-sys_info_prev.cpus[i].total_idle_time())/
                                        (sys_info.cpus[i].total_time( )-sys_info_prev.cpus[i].total_time( )) )/10.0;
    }
}

void topf::fill_procs( vector<topf::Proc_and_More>& procs, int size, SystemInfo& sys_info, SystemInfo& sys_info_prev,unsigned long long ttime, unsigned long long ttime_prev )
{
    int ip;
    for( size_t i=0; i<sys_info.processes.size( ); i++ )
    {
      ip=topf::index_find( sys_info_prev.processes,sys_info.processes[i].pid );
      if( ip != -1 )
      {
        procs[i].percent_cpu=100.0*(sys_info.processes[i].utime-sys_info_prev.processes[ip].utime+
                                    sys_info.processes[i].stime-sys_info_prev.processes[ip].stime)/(ttime-ttime_prev);
        procs[i].proc_time=double(sys_info.processes[i].utime+sys_info.processes[i].stime)/double(sysconf(_SC_CLK_TCK));
      }
      else
      {
        procs[i].percent_cpu=0.0;
        procs[i].proc_time=0.0;
      }
      procs[i].p=sys_info.processes[i];
    }
}

void topf::sort_procs( vector<topf::Proc_and_More>& procs, string& sort_opt )
{
      if( sort_opt == "PID" )
      {
        sort( procs.begin( ), procs.end( ), PIDcomp );
      }
      if( sort_opt == "CPU" )
      {
        sort( procs.begin( ), procs.end( ), CPUcomp );
      }
      if( sort_opt == "MEM" )
      {
        sort( procs.begin( ), procs.end( ), MEMcomp );
      }
      if( sort_opt == "TIME" )
      {
        sort( procs.begin( ), procs.end( ), TIMEcomp );
      }
}

string topf::get_proc_time( topf::Proc_and_More& proc, int mins, int hours ) 
{ 
    string sproc_time;
    double secs,hun_secs;
    stringstream ss1,ss2,ss3,ss4;
    hours=floor(proc.proc_time/(3600.0));
    ss1 << hours;
    mins=floor((proc.proc_time/(3600.0)-hours)*60.0);
    ss2 << mins;
    secs=((proc.proc_time/(3600.0)-hours)*60.0-mins)*60.0;
    ss3 << floor(secs);
    hun_secs=floor(round(100.0*(secs-floor(secs))));
    ss4 << hun_secs;
    sproc_time=ss1.str( ) + ":";
    if( mins < 10 )
    {
       sproc_time= sproc_time + "0" + ss2.str( ) + ":";
    }
    else
    {
       sproc_time= sproc_time + ss2.str( ) + ":";
    }
    if( secs < 10 )
    {
       sproc_time= sproc_time + "0" + ss3.str( ) + ".";
    }
    else
    {
       sproc_time= sproc_time + ss3.str( ) + ".";
    }
    if( hun_secs < 10 )
    {
       sproc_time= sproc_time + "0" + ss4.str( );
    }
    else
    {
       sproc_time= sproc_time + ss4.str( );
    } 
    return sproc_time;
}
