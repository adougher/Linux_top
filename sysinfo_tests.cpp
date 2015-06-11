/**
 * This file contains unit tests for the methods declared in sysinfo.h.
 *
 * You must complete the implementations of the method stubs in sysinfo.cpp in
 * whatever manner you choose, so long as these tests pass.
 */

#include "sysinfo.h"
#include "gtest/gtest.h"
#include <algorithm>

using namespace std;

bool z=0;

// Simple comparator function for ordering processes by their PIDs.
bool processComparator(const Process& p1, const Process& p2) {
  return p1.pid < p2.pid;
}


// Tests get_uptime().

TEST(GetUptime, CorrectValue) {
  EXPECT_DOUBLE_EQ(1527597.13, get_uptime());
}


// Tests get_load_average().

TEST(GetLoadAverage, OneMinuteLoadAvg) {
  EXPECT_DOUBLE_EQ(0.00, get_load_average().one_min);
}

TEST(GetLoadAverage, FiveMinuteLoadAvg) {
  EXPECT_DOUBLE_EQ(0.01, get_load_average().five_mins);
}

TEST(GetLoadAverage, FifteenMinuteLoadAvg) {
  EXPECT_DOUBLE_EQ(0.05, get_load_average().fifteen_mins);
}


// Tests get_memory_info()

TEST(GetMemoryInfo, TotalMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(8133452, info.total_memory);
}

TEST(GetMemoryInfo, FreeMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(5740368, info.free_memory);
}

TEST(GetMemoryInfo, BuffersMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(455696, info.buffers_memory);
}

TEST(GetMemoryInfo, CachedMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(1379468, info.cached_memory);
}

TEST(GetMemoryInfo, TotalSwap) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(16385272, info.total_swap);
}

TEST(GetMemoryInfo, FreeSwap) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(16385272, info.free_swap);
}


// Tests for get_cpu_info()

TEST(GetCpuInfo, NumCpuEntries) {
  vector<Cpu> cpus = get_cpu_info();
  EXPECT_EQ(9, cpus.size());
}

TEST(GetCpuInfo, UserTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(1002737LL, cpus[0].user_time);
  EXPECT_EQ(425755LL, cpus[1].user_time);
  EXPECT_EQ(57435LL, cpus[5].user_time);
}

TEST(GetCpuInfo, NiceTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(221960254LL, cpus[0].nice_time);
  EXPECT_EQ(28691658LL, cpus[1].nice_time);
  EXPECT_EQ(24910908LL, cpus[5].nice_time);
}

TEST(GetCpuInfo, SystemTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(284612LL, cpus[0].system_time);
  EXPECT_EQ(182240LL, cpus[1].system_time);
  EXPECT_EQ(8296LL, cpus[5].system_time);
}

TEST(GetCpuInfo, IdleTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(998565966LL, cpus[0].idle_time);
  EXPECT_EQ(123289112LL, cpus[1].idle_time);
  EXPECT_EQ(127751179LL, cpus[5].idle_time);
}

TEST(GetCpuInfo, IoWaitTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(48147LL, cpus[0].io_wait_time);
  EXPECT_EQ(31892LL, cpus[1].io_wait_time);
  EXPECT_EQ(2371LL, cpus[5].io_wait_time);
}

TEST(GetCpuInfo, IrqTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(7LL, cpus[0].irq_time);
  EXPECT_EQ(7LL, cpus[1].irq_time);
  EXPECT_EQ(0LL, cpus[5].irq_time);
}

TEST(GetCpuInfo, SoftIrqTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(25475LL, cpus[0].softirq_time);
  EXPECT_EQ(23374LL, cpus[1].softirq_time);
  EXPECT_EQ(592LL, cpus[5].softirq_time);
}

TEST(GetCpuInfo, StealTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(0LL, cpus[0].steal_time);
  EXPECT_EQ(0LL, cpus[1].steal_time);
  EXPECT_EQ(0LL, cpus[5].steal_time);
}

TEST(GetCpuInfo, GuestTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(0LL, cpus[0].guest_time);
  EXPECT_EQ(0LL, cpus[1].guest_time);
  EXPECT_EQ(0LL, cpus[5].guest_time);
}

TEST(GetCpuInfo, GuestNiceTime) {
  vector<Cpu> cpus = get_cpu_info();
  ASSERT_FALSE(cpus.empty());

  EXPECT_EQ(0LL, cpus[0].guest_nice_time);
  EXPECT_EQ(0LL, cpus[1].guest_nice_time);
  EXPECT_EQ(0LL, cpus[5].guest_nice_time);
}


// Tests for get_process(pid)

TEST(GetProcess, MemorySize) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(84858, process.size);
}

TEST(GetProcess, ResidentSize) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(807, process.resident);
}

TEST(GetProcess, SharedSize) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(670, process.share);
}

TEST(GetProcess, ProcessId) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(27897, process.pid);
}

TEST(GetProcess, ProcessFlags) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(4202496, process.flags);
}

TEST(GetProcess, UserTime) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(0, process.utime);
}

TEST(GetProcess, SystemTime) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(0, process.utime);
}

TEST(GetProcess, ChildUserTime) {
  Process process = get_process(1050, PROC_ROOT, z);
  EXPECT_EQ(3992909, process.cutime);
}

TEST(GetProcess, ChildSystemTime) {
  Process process = get_process(1050, PROC_ROOT, z);
  EXPECT_EQ(596, process.cstime);
}

TEST(GetProcess, RssLim) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(18446744073709551615UL, process.rsslim);
}

TEST(GetProcess, SigCatch) {
  Process process = get_process(27897, PROC_ROOT, z);
  EXPECT_EQ(81920, process.sigcatch);
}

TEST(GetProcess, Command) {
  Process process = get_process(56, PROC_ROOT, z);
  EXPECT_STRCASEEQ("(crypto)", process.comm);
}

TEST(GetProcess, CommandLine) {
  Process process = get_process(27859, PROC_ROOT, z);

  string expected = "/usr/bin/X :0 -auth "
      "/var/run/lightdm/root/:0 -nolisten tcp vt7 -novtswitch";

  EXPECT_EQ(expected, process.command_line);
}

TEST(GetProcess, CommandLineDefaultsToCommandWhenEmpty) {
  Process process = get_process(12, PROC_ROOT, z);
  string expected = "watchdog/1";

  EXPECT_EQ(expected, process.command_line);
}

TEST(GetProcess, Threads) {
  Process process = get_process(1566, PROC_ROOT, z);

  ASSERT_EQ(65, process.threads.size());

  EXPECT_EQ(1566, process.threads[0].tgid);
  EXPECT_EQ(1566, process.threads[42].tgid);
}

TEST(GetProcess, WithNestedBasePath) {
  Process process = get_process(1573, PROC_ROOT "/1566/task", z);

  EXPECT_EQ(1573, process.pid);
}

TEST(GetProcess, WithNestedBasePathThreads) {
  Process process = get_process(1573, PROC_ROOT "/1566/task", z);

  EXPECT_EQ(0, process.threads.size());
}


// Tests for get_all_processes()

TEST(GetAllProcesses, CorrectProcessContents) {
  vector<Process> processes = get_all_processes(PROC_ROOT);
  ASSERT_EQ(146, processes.size());

  sort(processes.begin(), processes.end(), processComparator);

  EXPECT_STRCASEEQ("(init)", processes[0].comm);
  EXPECT_STRCASEEQ("(kthreadd)", processes[1].comm);
  EXPECT_STRCASEEQ("(flush-8:0)", processes[144].comm);
  EXPECT_STRCASEEQ("(sshd)", processes[145].comm);
}

TEST(GetAllProcesses, SubdirectoryCount) {
  vector<Process> processes = get_all_processes(PROC_ROOT "/1566/task");
  ASSERT_EQ(65, processes.size());

  sort(processes.begin(), processes.end(), processComparator);

  EXPECT_EQ(1566, processes[0].pid);
  EXPECT_EQ(1567, processes[1].pid);
  EXPECT_EQ(1630, processes[63].pid);
  EXPECT_EQ(1631, processes[64].pid);
}


// Tests for get_system_info()

TEST(GetSystemInfo, NumTasks) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(146, sysinfo.num_processes);
}

TEST(GetSystemInfo, NumThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(204, sysinfo.num_threads);
}

TEST(GetSystemInfo, NumUserThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(121, sysinfo.num_user_threads);
}


TEST(GetSystemInfo, NumKernelThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(83, sysinfo.num_kernel_threads);
}

TEST(GetSystemInfo, NumRunning) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(2, sysinfo.num_running);
}

TEST(GetSystemInfo, Uptime) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_DOUBLE_EQ(get_uptime(), sysinfo.uptime);
}

TEST(GetSystemInfo, LoadAverage) {
  SystemInfo sysinfo = get_system_info();
  LoadAverage loadavg = get_load_average();

  EXPECT_DOUBLE_EQ(loadavg.one_min, sysinfo.load_average.one_min);
  EXPECT_DOUBLE_EQ(loadavg.five_mins, sysinfo.load_average.five_mins);
  EXPECT_DOUBLE_EQ(loadavg.fifteen_mins, sysinfo.load_average.fifteen_mins);
}

TEST(GetSystemInfo, CpuInfo) {
  SystemInfo sysinfo = get_system_info();
  vector<Cpu> cpus = get_cpu_info();

  ASSERT_EQ(cpus.size(), sysinfo.cpus.size());

  for (size_t i = 0; i < cpus.size(); i++) {
    EXPECT_EQ(cpus[i].idle_time, sysinfo.cpus[i].idle_time);
  }
}

TEST(GetSystemInfo, ProcessList) {
  SystemInfo sysinfo = get_system_info();
  vector<Process> processes = get_all_processes(PROC_ROOT);

  EXPECT_EQ(processes.size(), sysinfo.processes.size());
}
