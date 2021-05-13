/*
 * flextime.h
 *
 *  Created on: 23 nov. 2020
 *      Author: Johannes
 */

#ifndef SRC_FLEXTIME_HEADER_H_
#define SRC_FLEXTIME_HEADER_H_

#include <stdio.h>

//Macros
#define FIRST_ARGUMENT 1
#define ONE 1
#define TWO 2
#define THREE 3
#define WEEKS 2
#define DAYS 5
#define STAMPS 4
#define TIMES	7
#define ASCIIOFFSET 48
#define FULL_SIZE_FILE 411
#define MINUTES 60
#define MAXNAMESIZE 200
#define PERIODSIZE 7
#define TWO_WEEKS 80
#define REST_OF_WEEK 10
#define SIZE_OF_LINE 41
#define MODIFIED_DAYS 9
#define TEN 10
#define HUNDRED 100
#define OVER_HALF_OF_A_LINE 25

//Structures
typedef struct flextime{
	char *name;
	int id;
	int period;
	int day;
	int minutes;
}timestamp;

typedef struct Days{
	char *name;
	int id;
	int period;
	int week;
	int workhours;
	float workfraction;
	float totalworktime;
}Workday;


//Prototypes
/****************************************************************/
//Name
void open_file(char* argv[], FILE *fileptr);

//Purpose
// 	Checks if the program can reach the file in question.
//Input
//	Takes program argument string and an empty FILE pointer.
//Output
//	Prints to console whether or not the file could be reached.

/****************************************************************/
//Name
void take_raw_input(char buffer[], char* argv[]);
//Purpose
//	Reads the entire file character by character, finds filename in program-
//	argument. Places the entire string except for newlines in buffer.
//	Uses counter to create the size of buffer.
//Input
//	Filename from argv, buffer in main, size of counter.
//Output
//	Creates raw data in the form of characters.

/****************************************************************/
//Name
void make_ints(char buffer[], int intbuffer[], int counter);
//Purpose
//	Removes the commas and semi-colons while creating integers out of the char-array.
//	Putting the integers into a new array.
//Input
//	Takes the char-buffer, the int-buffer and counter.
//Output
//	Creates an array of integers for use in later functions.

/****************************************************************/
//Name
void assign_to_int_buffer(int *multibuffer[WEEKS][DAYS][STAMPS][TIMES], int intbuffer[]);
//Purpose
//	Puts the int-buffer into a multidimensional array,
//	This array takes the form of: 2 weeks, 5 days 4 stamps and 7 values/stamp.
//Input
//	Multi-dimensional integer-buffer initialized in main, and the int-buffer.
//Output
//	Filled multi-dim array allowing for creation of stamps.

/****************************************************************/
//Name
void assign_to_stamps(int multibuffer[WEEKS][DAYS][STAMPS][TIMES], timestamp time[], timestamp person);
//Purpose
//	Places the multi-dimensional integer-buffer and assigns them to the timestamp structure.
//Input
//	Takes multi-dimensional integer-buffer, an array of timestamps and
//	the person struct which handles name, id and period of each stamp.
//Output
//	Filled one-dimensional array of timestamps.

/****************************************************************/
//Name
void assign_time_multitime(timestamp multitime[WEEKS][DAYS][STAMPS], timestamp time[]);
//Purpose
//	Places the timestamps into a form of a multidimensional form 2 weeks, 5days, 4 stamps.
//Input
//	Takes multi-dimensional array of timestamps and the one-dimensional array of timestamps.
//Output
//	Multi-dimensional array of timestamps.

/****************************************************************/
//Name
void assign_workdays(Workday workdays[WEEKS][DAYS], timestamp multitime[WEEKS][DAYS][STAMPS], timestamp person);
//Purpose
//	Assembles the timestamps into structs called Workdays, basically removing one dimension.
//	Adding name, Id  and period for each stamp.
//Input
//	Takes mutli-dimensional workday structs, multi-dimensional timestamp array and the person struct.
//Output
//	Two dimensional array of workdays; format: 2 Weeks, 5 Days, almost complete information.

/****************************************************************/
//Name
void assign_weeks(Workday workdays[WEEKS][DAYS]);
//Purpose
//	Adds last part to the workdays, which week the day is in, either the first or second.
//Input
//	Takes the multi-dimensional array of workdays.
//Output
//	Completed information regarding a workday: Name, ID, Period, Week, Worked time in float-format.

/****************************************************************/
//Name
float calculate_workweek(Workday workdays[WEEKS][DAYS], float weektime);
//Purpose
//	creates a variable holding the total amount of time worked for the period in question.
//Input
//	Takes multi-dimensional array of workdays and and empty variable to be filled.
//Output
//	Returns the sum of all worked time during the period as a float.

/****************************************************************/
//Name
char* create_filename(timestamp person);
//Purpose
//	Takes the information in the person struct to dynamically create a new filename, adding .dat .
//Input
//	the timestamp person.
//Output
//	Returns the name of the new file in the form of a pointer to a string.

/****************************************************************/
//Name
void print_output(char* newfilename, timestamp person, float weektime, Workday workdays[WEEKS][DAYS]);
//Purpose
//	Prints the calculated worktime per day to a newly created file in the form of floats.
//Input
//	Takes: pointer to required filename, personal information from timestamp person, total worktime.
//	The worked time per day.
//Output
//	Creates a file with binary information holding ten values of the worked time.

/****************************************************************/
//Name
timestamp handle_arguments(int argc, char *argv[], timestamp person);
//Purpose
//	Checks whether or not an argument has been passed to the program.
//	Adds personal information to a struct.
//Input
//	Argument counter, and pointers to said arguments and empty timestamp struct to be filled.
//Output
//	Returns a struct filled with personal information and signals the status of arguments.

/****************************************************************/
//Name
int measure_size(int counter, char* argv[]);
//Purpose
//	Measures the length of the file by reading the characters EXCEPT for new-lines.
//Input
//	Takes the program argument to open, and an empty integer counter.
//Output
//	Returns the size (in chars) of the file in the form of an integer counter.

/****************************************************************/
//Name
int measure_line(int line_size_counter, char* argv[]);
//Purpose
//	Measures the length of the first individual line in the file.
//Input
//	Takes the empty counter variable and the program argument.
//Output
//	Returns length of the first line.

/****************************************************************/
//Name
int measure_total_size(int totalsize, char *argv[]);
//Purpose
//	Measures the total length of the file INCLUDING new-line characters.
//Input
//	Takes empty container and reference to open file.
//Output
//	Returns the total length of a file.

/****************************************************************/
//Name
int count_newlines(char* argv[]);
//Purpose
//	Counts the amounts of new-line characters in the file.
//Input
//	Takes reference on which file to open, argv.
//Output
//	Returns value to console.

/****************************************************************/
//Name
void take_modified_raw_input(char *argv[], char buffer[]);
//Purpose
//	Stores the entire file including new-lines to the buffer.
//Input
//	Takes the program argument for file-opening and the buffer.
//Output
//	Reads the entire file into the buffer.

/****************************************************************/
//Name
void read_and_present_file(char *newfilename, timestamp person);
//Purpose
//	Opens the new binary file, reads it float by float, stores it in array.
//	Presents the data stored as formatted console output.
// 	Calculates a new workweek from collected floats.
//Input
//	Takes the filename to be opened as char pointer and the personal information.
//Output
//	Presents output to screen.

/****************************************************************/
//Name
int find_values(char buffer[], int leave_of_absence);
//Purpose
//	Checks if there are any double-new-lines, basically if there are entire days missing.
//Input
//	Takes the buffer and an empty container.
//Output
//	Returns the day where the day is missing.
/****************************************************************/
//Name
int find_values_2(char buffer[], int half_day, int line_size_counter);
//Purpose
//	Checks the file to see if there are stamps missing, counts the distance between -
//	Two newlines, checks if it's not a leave-of absence, and if so, reports it.
//Input
//	Takes buffer, half day and a counter.
//Output
//	Returns the day in which the half-day was found.

/****************************************************************/
//Name
void assign_to_modified_int_buffer(int multibuffer[MODIFIED_DAYS][STAMPS][TIMES], int intbuffer[], int leave_of_absence, int half_day, int stamp);
//Purpose
//	Used if the file is not complete, puts the integers into a multi-dim buffer.
//	Special care is given to finding and sorting according to if leave of absence or
// 	Use of flextime is found.
//Input
//	Takes one and Multi-dim int-buffer, values for where Leave of absence and flextime usage was seen.
//Output
//	Provides a modified integer-buffer.
/****************************************************************/
//Name
int find_values_3(char buffer[], int line_size_counter);
//Purpose
//	Finds where and if flextime has been used, hence the removal of two stamps.
//	Finds this through measuring the distance between two new-lines.
//Input
//	Takes the character buffer and the length of a line.
//Output
//	Returns the line where the newlines appear too frequently.

#endif /* SRC_FLEXTIME_HEADER_H_ */
