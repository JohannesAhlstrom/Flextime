/*
 * flextime_main.c
 *
 *  Created on: 23 nov. 2020
 *      Author: Johannes
 */

#include "flextime_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
	setbuf(stdout,NULL);
	FILE *fileptr;
	timestamp person;
	char *newfilename =NULL;
	int counter= 0, line_size_counter = 0;
	int totalsize = 0, newline_counter = 0;
	int leave_of_absence = 0, half_day = 0, stamp = 0;
	float weektime =0;

	//Perform measurements and present connection information.
	open_file(argv, fileptr);
	person = handle_arguments(argc, argv, person);
	totalsize = measure_total_size(totalsize,argv);
	newline_counter = count_newlines(argv);
	counter = measure_size(counter, argv);
	line_size_counter = measure_line(line_size_counter, argv);


	//Check if the file contains imperfections.
	if(totalsize < FULL_SIZE_FILE)
	{
	//Take entire input, find leave of absence and flex time usage.
	char buffer[totalsize];
	take_modified_raw_input(argv, buffer);
	leave_of_absence = find_values(buffer, leave_of_absence);
	half_day = find_values_2(buffer, half_day,line_size_counter);
	stamp = find_values_3(buffer, line_size_counter);

	//Remove newlines, make integers.
	char buffer2[counter];
	int intbuffer[280];
	take_raw_input(buffer2, argv);
	make_ints(buffer2, intbuffer, counter);

	//Assign the int-buffer to a multi-dimensional int-array.
	int multibuffer[WEEKS*DAYS][STAMPS][TIMES];
	assign_to_modified_int_buffer(multibuffer, intbuffer, leave_of_absence, half_day,stamp);

	//Assign the multi-int array to timestamp one-dim struct array.
	timestamp time[WEEKS*DAYS*STAMPS];
	assign_to_stamps(multibuffer,time, person);

	//Create multi-dim timestamp array.
	timestamp multitime[WEEKS*DAYS][STAMPS];
	assign_time_multitime(multitime,time);

	//Assign each stamp as day in a specific week.
	Workday workdays[WEEKS*DAYS];
	assign_workdays(workdays, multitime, person);
	workdays[leave_of_absence-1].totalworktime=8;

	//Add information of which day belongs to which week.
	assign_weeks(workdays);

	//Calculates the total time worked during the period.
	weektime = calculate_workweek(workdays, weektime);

	//Dynamically create filename.
	newfilename = create_filename(person);

	//Calculate and Write to binary file.
	print_output(newfilename, person, weektime, workdays);

	//Read from said file and present output to console.
	read_and_present_file(newfilename, person);
	free(newfilename);

	}
	else{
	char buffer[counter];
	//Start treating string as char array.
	take_raw_input(buffer, argv);

	//Transform to int-array.
	int intbuffer[counter];
	make_ints(buffer,intbuffer,counter);

	//Put int into multi-dimensional array.
	int multibuffer [WEEKS][DAYS][STAMPS][TIMES];
	assign_to_int_buffer(multibuffer, intbuffer);

	//Assign the multi-int array to timestamp one-dim struct array.
	timestamp time[WEEKS*DAYS*STAMPS];
	assign_to_stamps(multibuffer,time, person);

	//Create multi-dim timestamp array.
	timestamp multitime[WEEKS][DAYS][STAMPS];
	assign_time_multitime(multitime,time);

	//Assign each stamp as day in a specific week.
	Workday workdays[WEEKS][DAYS];
	assign_workdays(workdays, multitime, person);

	//Add information of which day belongs to which week.
	assign_weeks(workdays);

	//Calculates the total time worked during the period.
	weektime = calculate_workweek(workdays, weektime);

	//Dynamically create filename.
	newfilename = create_filename(person);

	//Calculate and Write to binary file.
	print_output(newfilename, person, weektime, workdays);

	//Read from said file and present output to console.
	read_and_present_file(newfilename, person);
	free(newfilename);
	}
	return 0;
}
