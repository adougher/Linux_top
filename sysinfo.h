/**
 * Contains function signatures of methods that are intended to retrieve data
 * from the /proc file system.
 *
 * You are not allowed to modify any of these existing method signatures, though
 * you may add additional methods if you want.
 */

#pragma once
#include <vector>
#include "types/cpu.h"
#include "types/memory_info.h"
#include "types/process.h"
#include "types/system_info.h"


// The root path of the proc filesystem to use.
// This value is overridden (via the makefile) for the unittests with the path
// to the fake proc directory in this repository.
#ifndef PROC_ROOT
# define PROC_ROOT "/proc"
#endif


/**
 * Returns the amount of time, in seconds, that the machine has been running.
 */
double get_uptime();


/**
 * Returns a LoadAverage instance populated with the average load over the last
 * 1, 5, and 15 minutes.
 */
LoadAverage get_load_average();


/**
 * Returns a fully populated MemoryInfo instance, containing information about
 * the system's memory usage and available memory.
 */
MemoryInfo get_memory_info();


/**
 * Returns a vector with one Cpu entry for each processor present in the system,
 * in the order the system sees them, with an additional entry at the beginning
 * of the vector representing the overal system CPU usage.
 */
std::vector<Cpu> get_cpu_info();


/**
 * Returns a Process instance representing the process with the given process
 * ID (pid). The data for the process should be loaded from the directory formed
 * by joining the given basedir with the process ID (basedir/pid).
 */
Process get_process(int pid, const char* basedir, bool& is_dead);


/**
 * Returns a vector consisting of one Process entry for each process currently
 * present on the system (if passed the root of the /proc file system),  or one
 * entry for each thread in a single process (if passed a /proc/<pid>/task
 * basedir).
 */
std::vector<Process> get_all_processes(const char* basedir);


/**
 * Returns a fully populated SystemInfo instance aggregating all required data
 * about the system at a given point in time.
 */
SystemInfo get_system_info();
