#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include "types/process.h"
#include "types/system_info.h"

#ifndef TOP_FUNCTIONS_H
#define TOP_FUNCTIONS_H

using namespace std;

namespace topf
{
  // This strust is utilizied in sorting the procceses on the
  // appropriate fields.
  struct Proc_and_More
  {
    Process p;
    double percent_cpu;
    double proc_time;
  };

  /* This function searches a vector of Processes for one that
   * has the specified PID an returns its index.  If none is 
   * found, negative one is returned.
   */
  int index_find( vector<Process>& procs, int curpid );

  /* This functions converts the pages of a process to
   * the size in bytes.  If the size is more than five 
   * digits, truncation occurs for the last three
   * digits and 'm' is appended to the number.
   */
  string get_pages_bytes( long rss );

  /* This function retrieves the needed information from 
   * the option flags, given some were passed. Exits
   * if the arguments are not valid.
   */
  void get_options( float& delay_time, int& do_sort, int& help, string& sort_opt, int argc, char** argv );

  // Converts time in clock ticks to HH:MM.
  string get_time( );

  // Get current number of users
  int get_num_users( SystemInfo& sys_info );

  /* Computes percentages of user, system and idle time
   * for each CPU and for the whole collection.
   */
  void get_cpu_percent( SystemInfo& sys_info,SystemInfo& sys_info_prev,unsigned long long& ttime,
                        unsigned long long& ttime_prev,double* buffer);

  /* This function fills a vector with the Proc_and_More 
   * data structure associated with each process.  It
   * contains all of the information of a process data
   * data structure along with percent CPU and processor
   * time running.
   */
  void fill_procs( vector<Proc_and_More>& procs, int size, SystemInfo& sys_info, SystemInfo& sys_info_prev,
                   unsigned long long ttime, unsigned long long ttime_prev );
  /* This function sorts the process based on the specified
   * field retrieved from the passed arguments.
   */
  void sort_procs( vector<Proc_and_More>& procs, string& sort_opt );

  /* This function is essentially like get_time(), but the
   * resolution is in hundredths of a second.
   */
  string get_proc_time( Proc_and_More& proc, int mins, int hours );
};
#endif
