#include "sysinfo.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>

using namespace std;


double get_uptime() {
  ifstream fs;
  fs.open("proc/uptime");
  stringstream strStream;
  strStream << fs.rdbuf();
  string proc_uptime = strStream.str();
  return atof((proc_uptime.substr( 0, proc_uptime.find_first_of( " " ) )).c_str( ));
}


LoadAverage get_load_average() {
  ifstream fs;
  fs.open("proc/loadavg");
  stringstream strStream;
  strStream << fs.rdbuf();
  string load_avg = strStream.str();
  LoadAverage l_avg;
  l_avg.one_min=atof((load_avg.substr( 0, load_avg.find_first_of( " " ) )).c_str( ));
  load_avg.erase( 0, load_avg.find_first_of( " " )+1);
  l_avg.five_mins=atof((load_avg.substr( 0, load_avg.find_first_of( " " ) )).c_str( ));
  load_avg.erase( 0, load_avg.find_first_of( " " )+1);
  l_avg.fifteen_mins=atof((load_avg.substr( 0, load_avg.find_first_of( " " ) )).c_str( ));
  load_avg.erase( 0, load_avg.find_first_of( " " )+1);
  return l_avg;
}


MemoryInfo get_memory_info() {
  
  ifstream fs;
  fs.open("proc/meminfo");
  stringstream strStream;
  strStream << fs.rdbuf();
  string mem_info = strStream.str();
  MemoryInfo meminfo;
  string line=mem_info.substr( mem_info.find("MemTotal"), mem_info.find_first_of("\n",mem_info.find("MemTotal") )-mem_info.find("MemTotal") );
  meminfo.total_memory=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
				line.find_first_of( "0123456789" ) )).c_str( ));
  line=mem_info.substr( mem_info.find("MemFree"), mem_info.find_first_of("\n",mem_info.find("MemFree") )-mem_info.find("MemFree") );
  meminfo.free_memory=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
				line.find_first_of( "0123456789" ) )).c_str( ));
  line=mem_info.substr( mem_info.find("Buffers"), mem_info.find_first_of("\n",mem_info.find("Buffers") )-mem_info.find("Buffers") );
  meminfo.buffers_memory=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
				line.find_first_of( "0123456789" ) )).c_str( ));
  line=mem_info.substr( mem_info.find("Cached"), mem_info.find_first_of("\n",mem_info.find("Cached") )-mem_info.find("Cached") );
  meminfo.cached_memory=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
				line.find_first_of( "0123456789" ) )).c_str( ));
  line=mem_info.substr( mem_info.find("SwapTotal"), mem_info.find_first_of("\n",mem_info.find("SwapTotal") )-mem_info.find("SwapTotal") );
  meminfo.total_swap=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
				line.find_first_of( "0123456789" ) )).c_str( ));
  line=mem_info.substr( mem_info.find("SwapFree"), mem_info.find_first_of("\n",mem_info.find("SwapFree") )-mem_info.find("SwapFree") );
  meminfo.free_swap=atoll((line.substr( line.find_first_of( "0123456789" ), line.find_first_not_of( "0123456789",line.find_first_of( "0123456789" ) )-
 				line.find_first_of( "0123456789" ) )).c_str( ));
  return meminfo;
}


vector<Cpu> get_cpu_info() {
  ifstream fs;
  fs.open("proc/stat");
  stringstream strStream;
  strStream << fs.rdbuf();
  string cpu_info = strStream.str();
  Cpu c_p_u;
  vector<Cpu> cpus;
  while( cpu_info.find( "cpu" ) == 0 )
  {
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    if( cpu_info[0]==' ' )
    {
      cpu_info.erase( 0,1 );
    }
    c_p_u.user_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.nice_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.system_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.idle_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.io_wait_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.irq_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.softirq_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.steal_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.guest_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( " " ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( " " )+1);
    c_p_u.guest_nice_time=atoll((cpu_info.substr( 0, cpu_info.find_first_of( "\n" ) )).c_str( ));
    cpu_info.erase( 0, cpu_info.find_first_of( "\n" )+1);
    cpus.push_back( c_p_u );
  } 
  return cpus;
}


Process get_process(int pid, const char* basedir, bool & process_died) {
  ifstream fs1, fs2;  // Separate file streams for each needed proc file
  stringstream strStream1, strStream2; // string streams for parsing  

  // convert pid to string
  string spid;
  ostringstream temp;
  temp << pid;
  spid=temp.str( );

  string process;
  Process proc;

  // Retrieve needed fields from /statm
  string dir=string(basedir)+"/"+spid+"/statm";
  FILE* pfile=fopen(dir.c_str( ),"r");
  if( pfile == NULL )
  {
    process_died=1;
    return proc;
  }
  fscanf(pfile,"%ld %ld %ld %ld %ld %ld %ld", 
         &proc.size,&proc.resident,&proc.share, 
         &proc.trs,&proc.lrs,&proc.drs,&proc.dt );
  fclose( pfile );

  // Retrieve command line
  dir=string(basedir)+"/"+spid+"/cmdline";
  fs1.open(dir.c_str( ));
  if( fs1.is_open( ) == 0 )
  {
    process_died=1;
    return proc;
  }
  strStream1 << fs1.rdbuf();
  process=strStream1.str();
  unsigned sz=process.size( );
  if( sz > 0 )
  {
    proc.command_line=process;
    for( size_t i=0; i<sz-1; i++ )
    {
      if( proc.command_line[i] == '\0' )
      {
        proc.command_line[i]=' ';
      }
    }
    proc.command_line.erase( sz-1,1 );
  }
  fs1.close( );

  // Retrieve thread ID
  dir=string(basedir)+"/"+spid+"/status";
  fs2.open(dir.c_str( ));
  if( fs2.is_open( ) == 0 )
  {
    process_died=1;
    return proc;
  }
  strStream2 << fs2.rdbuf();
  process = strStream2.str();
  string line=process.substr( process.find("Tgid"), 
              process.find_first_of("\n",process.find("Tgid") )-
              process.find("Tgid") );
  proc.tgid=atoi((line.substr( line.find_first_of( "0123456789" ), 
                  line.find_first_not_of( "0123456789",
                  line.find_first_of( "0123456789" ) )-
	          line.find_first_of( "0123456789" ) )).c_str( ));
  line=process.substr( process.find("Uid"), process.find_first_of("\n",process.find("Uid") )-
                       process.find("Uid") );
  int us_id=atoi((line.substr( line.find_first_of( "0123456789" ),
                  line.find_first_not_of( "0123456789",
                  line.find_first_of( "0123456789" ) )-
                  line.find_first_of( "0123456789" ) )).c_str( ));
  passwd* pw=getpwuid(us_id);
  string s_name(pw->pw_name);
  proc.user_name=s_name;
  fs2.close( );

  // Retrieve a majority of the process fields from /stat
  dir=string(basedir)+"/"+spid+"/stat";
  pfile=fopen(dir.c_str( ),"r");
  if( pfile == NULL )
  {
    process_died=1;
    return proc;
  }
  fscanf(pfile,"%d %100s %1c %d %d %d %d %d %u %lu %lu %lu %lu"
                "%lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu"
                "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu"
                "%d %d %u %u %llu %lu %ld",
         &proc.pid,proc.comm,&proc.state,&proc.ppid,&proc.pgrp,
         &proc.session,&proc.tty_nr,&proc.tpgid,&proc.flags,
         &proc.minflt,&proc.cminflt,&proc.majflt,&proc.cmajflt,
         &proc.utime,&proc.stime,&proc.cutime,&proc.cstime,
         &proc.priority,&proc.nice,&proc.num_threads,&proc.itrealvalue,
         &proc.starttime,&proc.vsize,&proc.rss,&proc.rsslim,
         &proc.startcode,&proc.endcode,&proc.startstack,&proc.kstkesp,
         &proc.kstkeip,&proc.signal,&proc.blocked,
         &proc.sigignore,&proc.sigcatch,&proc.wchan,
         &proc.nswap,&proc.cnswap,&proc.exit_signal,
         &proc.processor,&proc.rt_priority,&proc.policy,
         &proc.delayacct_blkio_ticks,&proc.guest_time,
         &proc.cguest_time);
  fclose( pfile );

  string proc_comm(proc.comm);
  int i=proc_comm.size();
  while( i < 100 )
  {
    proc.comm[i]='\0';
    i++;
  }
  if( proc.command_line.size( ) == 0 )
  {
    proc.command_line=string(proc.comm);
    proc.command_line.erase(0,1);
    proc.command_line.erase( proc.command_line.size( )-1,1);
  }

  // Note that the recursion depth will only be one since
  // the threads do not have a /task folder
  dir=string(basedir)+"/"+spid+"/task";
  DIR* directory = opendir(dir.c_str( ));
  if( directory )
  {
    proc.threads=get_all_processes(dir.c_str( ));
  }
  closedir(directory);

  return proc;
}


vector<Process> get_all_processes(const char* basedir) {
  DIR* dir = opendir(basedir);
  vector<string> results;
  for (dirent* current = readdir(dir); current; current = readdir(dir)) {
    results.push_back(current->d_name);
  }
  closedir(dir);
  
  vector<Process> processes;
  int pid;
  bool is_dead=0;
  Process proc;
  for( size_t i=0; i<results.size( ); i++ )
  {
    if( results[i].find_first_not_of( "0123456789" ) == string::npos )
    {
      pid=atoi(results[i].c_str( ) );
      proc=get_process( pid, basedir, is_dead );
      if( !is_dead )
      {
        processes.push_back( proc );
      }
      is_dead=0;
    }
  }
  return processes;
}

SystemInfo get_system_info() {
  SystemInfo sys_info;
  sys_info.uptime=get_uptime( );
  sys_info.load_average=get_load_average( );
  sys_info.cpus=get_cpu_info( );
  sys_info.processes=get_all_processes(PROC_ROOT);
  sys_info.num_processes=sys_info.processes.size( );
  unsigned threads=0;
  unsigned u_threads=0;
  unsigned k_threads=0;
  unsigned running=0;
  for( size_t i=0; i<sys_info.processes.size( ); i++ )
  {
    if( sys_info.processes[i].state == 'R' )
    {
      running++;
    }
    for( size_t j=0; j<sys_info.processes[i].threads.size( ); j++ )
    {
      if( sys_info.processes[i].threads[j].is_kernel_thread() )
      {
        k_threads++;
      }
      if( sys_info.processes[i].threads[j].is_user_thread() )
      {
        u_threads++;
      }
      if( sys_info.processes[i].threads[j].is_thread() )
      {
        threads++;
      }
    }
  }
  sys_info.num_threads=threads;
  sys_info.num_user_threads=u_threads;
  sys_info.num_kernel_threads=k_threads;
  sys_info.num_running=running;

  return sys_info;
}
