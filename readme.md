Welcome to Flextime!
----------------------------------------------------------------------------------
Instructions:
In order to run the program:
	Click the "Run_me_first_flextime_compiler.sh" file
	Click the filename of the .bat file you want to see presented.

Note: I separated the compilation and execution-files for cleaner presentation.

----------------------------------------------------------------------------------

Flextime
This is a purely educational program written in C.

This program locates and parses a textfile filled with timestamps 
separated using a common CSV method.
The filename is used to track period of time, worker-id and name.

Ek has worked a full week schedule, Bjork has not, this requires the program 
to be able to handle missing information in the stamps.

A line in the .txt file looks like this:
307,07,23;307,11,44;307,12,31;307,16,30;

Each line in the .txt file contains four stamps.
Stamps are separated by ";".
Individual values are separated by ",".

Stamps are then put into structs containing:

Day of year
Hour
Minutes

For example:
On the 307th day.
A person clocked in at 07:23, left for lunch at 11:44.
Returned from lunch at 12:31, and left work at 16:30.

The program locates and parses the information using a multi-dimentional array
running in four-level loops to handle, prepare and execute calculations.
The presentation contains the total worktime during each day of the week
and how much overtime has been accumulated or flextime used by the worker
benchmarked from a 40 hour workweek.

The output is a .dat file containing the calculations.

@Author: Johannes Ahlström
