Andrew Dougherty
CSCI 442, Spring 2014
Project 2

sysinfo.h:
This is the header file for the system information gathering 
functionality.

sysinfo.cpp:
This file contains the implementation for the system information
gathering functionality.  It includes methods for retrieving
information about the uptime, memory and processes from the 
proc file system.

sysinfo_tests.cpp:
This file contains unit tests for the sysinfo functionality.

top_functions.h:
This is the header file for the top helper functions and a 
struct which is essentially an expanded process.

top_functions.cpp:
This file contains the implementation for the top helper
functions.  It includes an methods for retrieving a process
from a vector given a pid, a method for converting number
of pages to bytes, a method for parsing valid options, a
method for getting the current clock time, a method to
calculate the current percent user time, system time and
idle time for each CPU and for the system, given a propery
sized blank array, a method to populate a vector of the
process like data structures mentioned above, a method to
sort such a vector given a valid sort key and a method
to get the total runtime for a process in the format
  HH:MM:SS.CC

*******************************************************

The optimal terminal size is about half the monitor.

I spent about 12-14 hours total on this project.

*******************************************************

*******************************************************
Extra Credit Features:

My top displays the current time, the number of users
including system users, the swap memory info and
the the user associated with each process.

*******************************************************

****EDIT: This error has been fixed now****

It must be noted that on rare occasion, my top crashes due
to a string.substr( ) method invalid read error.  Sometimes
it can run for an hour without incident, sometimes only for 
a minute. I've deduced using valgrind that it originates from 
somewhere in the get_process( ) method, but it does not 
always occur.

I coded the method using stl strings for parsing instead of
fscanf, which seems to be a more robust and elegant 
solution.  Please don't dock me for too much points. o)
(That's Ermal the Cyclops)

****EDIT: This error has been fixed now****
