/*
 * flextimefunctions.c
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

//Observera: Vissa av funktionerna innehåller utkommenterade Debugg-meddelanden för enkel debugging.

void open_file(char* argv[], FILE *fileptr)
{
	char *openfile;
	openfile =malloc(strlen(argv[FIRST_ARGUMENT]));
	strcpy(openfile,argv[FIRST_ARGUMENT]);
	fileptr = fopen(openfile, "r");
	if(fileptr==NULL)
	{
		printf("\nCould not open file\nPlease verify path and filename\n.");
		exit(1);
	}
	else
		printf("\nFile found and opened successfully");
	fclose(fileptr);
	free(fileptr);

}

void take_raw_input(char buffer[], char* argv[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	char ch;
	int i =0;
	do
	{
		ch = fgetc(fileptr);
		if(ch != EOF && ch != '\n')
		{
			buffer[i] = ch;
			i++;
		}
	}while(ch != EOF);
	fclose(fileptr);
}

void make_ints(char buffer[], int intbuffer[], int counter)
{
	for(int i=0, j=0; i<counter;i++)
	{
		if(isdigit(buffer[i]))
		{
			intbuffer[j]=buffer[i]-ASCIIOFFSET;
			j++;
		}
	}

}

void assign_to_int_buffer(int *multibuffer[WEEKS][DAYS][STAMPS][TIMES], int intbuffer[])
{
	int l =0;
	for(int i = 0; i < WEEKS; i++)
		for(int j = 0; j < DAYS; j++)
			for(int k = 0; k < STAMPS; k++ )
				for(int m= 0; m < TIMES; m++)
				{
					multibuffer[i][j][k][m] = intbuffer[l];
					l++;
				}
}

void assign_to_stamps(int multibuffer[WEEKS][DAYS][STAMPS][TIMES], timestamp time[], timestamp person)
{
	int n = 0;
	int daysum=0;
	int hoursum = 0;
	int minutesum = 0;
	for(int i = 0; i<WEEKS; i++)
		for(int j = 0; j < DAYS; j++)
			for(int k = 0; k < STAMPS; k++)
			{
				daysum=0;
				hoursum = 0;
				minutesum = 0;
				for(int m = 0; m < TIMES; m++)
				{
					switch(m)
					{
					case 0: {daysum += multibuffer[i][j][k][m]*HUNDRED; break;}
					case 1: {daysum += multibuffer[i][j][k][m]*TEN; break;}
					case 2: {daysum += multibuffer[i][j][k][m]; break;}
					case 3: {hoursum += multibuffer[i][j][k][m]*10; break;}
					case 4: {hoursum += multibuffer[i][j][k][m]; break;}
					case 5: {minutesum += multibuffer[i][j][k][m]*TEN; break;}
					case 6: {
						minutesum += multibuffer[i][j][k][m];
						time[n].id = person.id;
						time[n].name = person.name;
						time[n].period = person.period;
						time[n].day = daysum;
						time[n].minutes =(hoursum*MINUTES)+(minutesum);
						n++;
						break;
					}
					}
				}
			}
}

void assign_time_multitime(timestamp multitime[WEEKS][DAYS][STAMPS], timestamp time[])
{
	int l = 0;
	for(int i = 0; i < WEEKS; i++)
		for(int j = 0; j<DAYS; j++)
			for(int k = 0; k <STAMPS; k++)
			{
				multitime[i][j][k] = time[l];
				l++;
			}
}

void assign_workdays(Workday workdays[WEEKS][DAYS], timestamp multitime[WEEKS][DAYS][STAMPS], timestamp person)
{
	int workdaysum =0, k = 0;
	for(int i = 0; i <WEEKS; i++)
	{
		workdaysum = 0;
		for(int j = 0; j < DAYS; j++)
		{
			workdaysum = (multitime[i][j][k+THREE].minutes - multitime[i][j][k].minutes)-(multitime[i][j][k+TWO].minutes - multitime[i][j][k+ONE].minutes);
			workdays[i][j].name = person.name;
			workdays[i][j].id = person.id;
			workdays[i][j].period =person.period;
			workdays[i][j].workhours = workdaysum/(int)MINUTES;
			workdays[i][j].workfraction = (float)((workdaysum%MINUTES) /(float)MINUTES);
			workdays[i][j].totalworktime = (float)(workdays[i][j].workhours)+(float)(workdays[i][j].workfraction);
		}
	}
}

void assign_weeks(Workday workdays[WEEKS][DAYS])
{
	for(int i = 0; i<WEEKS; i++)
		for(int j = 0; j<DAYS; j++)
		{
			if(i==0)
				workdays[i][j].week = (i+1);
			else if(i == 1)
				workdays[i][j].week = (i+1);
		}
}
float calculate_workweek(Workday workdays[WEEKS][DAYS], float weektime)
{
	for(int i = 0; i < WEEKS; i++)
		for(int j = 0; j <DAYS; j++)
		{
			weektime += workdays[i][j].totalworktime;
		}
	return weektime;
}

char* create_filename(timestamp person)
{
	char namebuffer[MAXNAMESIZE],*temp;
	char *tempori =(char*)malloc(PERIODSIZE);
	temp = tempori;
	strcpy(namebuffer,person.name);
	strcat(namebuffer,"_");
	sprintf(temp,"%d",person.id);
	strcat(namebuffer,temp);
	strcat(namebuffer,"_");
	sprintf(temp,"%d",person.period);

	strcat(namebuffer,temp);
	strcat(namebuffer,".dat");

	char *ori =(char*)malloc(strlen(namebuffer)+1);
	strcpy(ori,namebuffer);

	if(ori == NULL)
		printf("\nERROR, could not allocate memory");
	free(tempori);
	return ori;
}

void print_output(char* newfilename, timestamp person, float weektime, Workday workdays[WEEKS][DAYS])
{
	FILE *newfile;
	newfile = fopen(newfilename,"wb");
	if(newfile == NULL)
	{
		printf("\nERROR, file could not be created");
		exit(1);
	}
	else
		printf("\nSUCCESS, file created and reached correctly\n");
	for(int i = 0; i<WEEKS;i++)
	{
		for(int j = 0; j < DAYS; j++)
		{
			fwrite(&workdays[i][j].totalworktime, sizeof(float),1,newfile);
		}
	}
	fclose(newfile);
}

timestamp handle_arguments(int argc, char *argv[], timestamp person)
{
	char *ori = malloc((char*)strlen(argv[FIRST_ARGUMENT]));
	char *filenameptr = ori;
	char *tempori = malloc((char*)strlen(argv[FIRST_ARGUMENT]));
	char *tempptr = tempori;
	printf("\nProgram found at: %s", argv[0]);
	printf("\nArgument: %s", argv[FIRST_ARGUMENT]);
	if(argc ==1)
	{
		printf("\nNo additional arguments provided");
	}
	else if(argc>1)
	{
		if(filenameptr==NULL || argv[FIRST_ARGUMENT]==NULL)
		{
			printf("\nERROR, Either Filename or Argv is NULL");
		}
		else
		{
			strcpy(filenameptr,argv[FIRST_ARGUMENT]);
			filenameptr = ori;
			if(person.name == NULL)
				printf("\nPerson.name = NULL");
			else if(filenameptr ==NULL)
				printf("\nFilename PTR = NULL");
			filenameptr = ori;
			char *tempptr2 = ori;
			strcpy(tempptr2,strtok(filenameptr,"_"));
			tempptr=tempori;
			char *personptr = malloc ((char*) strlen(tempptr));
			personptr=tempptr2;

			strcpy(tempptr,strtok(NULL,"_"));
			person.id=atoi(tempptr);
			strcpy(tempptr, strtok(NULL, "."));
			person.period = atoi(tempptr);

			person.name =personptr;
		}
	}
	else
		printf("\nCould not handle the argument string");
	return person;
}

int measure_size(int counter, char* argv[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	if(fileptr == NULL)
		printf("\nThe fileptr is NULL");
	char ch;
	do
	{
		ch = fgetc(fileptr);
		if(ch != '\n')
		{
			counter++;
		}
		else
		{
			continue;
		}
	}while(ch !=  EOF);
	//printf("\nThe total Length of the file is: %d chars", counter);
	rewind(fileptr);
	fclose(fileptr);
	return counter;
}

int measure_line(int line_size_counter, char* argv[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	char ch;
	do
	{
		ch = fgetc(fileptr);
		line_size_counter++;
	}while(ch != '\n');
	//printf("\nThe length of the first line is: %d chars", line_size_counter);
	rewind(fileptr);
	fclose(fileptr);
	return line_size_counter;
}

int measure_total_size(int totalsize, char *argv[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	if(fileptr == NULL)
		printf("\nThe fileptr is NULL");
	char ch;
	do
	{
		ch = fgetc(fileptr);
		totalsize++;
	}while(ch !=  EOF);
	//printf("\nThe total length of the file is: %d chars", totalsize);
	rewind(fileptr);
	fclose(fileptr);
	return totalsize;
}
int count_newlines(char* argv[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	int newline_counter =0;
	if(fileptr == NULL)
		printf("\nThe fileptr is NULL");
	char ch;
	do
	{
		ch = fgetc(fileptr);
		if(ch == '\n')
			newline_counter++;
	}while(ch !=  EOF);
	//printf("\nThe total number of New-lines in the file is: %d chars", newline_counter);
	rewind(fileptr);
	fclose(fileptr);
	return newline_counter;
}

void take_modified_raw_input(char *argv[], char buffer[])
{
	FILE *fileptr = fopen(argv[FIRST_ARGUMENT], "r");
	char ch;
	int i =0;
	do
	{
		ch = fgetc(fileptr);
		if(ch != EOF)
		{
			buffer[i] = ch;
			i++;
		}
	}while(ch != EOF);
	fclose(fileptr);
}

void read_and_present_file(char *newfilename, timestamp person)
{
	FILE *fileptr;
	float readnumber,weektime = 0, storenumber[WEEKS*DAYS];
	fileptr = fopen(newfilename,"rb");
	if( (fileptr = fopen(newfilename,"rb"))==NULL){
		printf("\nUnable to read from the binary file");
		exit(1);
	}
	int i = 0;
	while(fread(&readnumber, sizeof(float), 1, fileptr)==1){
		storenumber[i++]=readnumber;
	}
	fclose(fileptr);

	printf("\n");
	printf("Name: %s\t Employee no:%d Period:%d\n",person.name, person.id, person.period);
	printf("\n");
	printf("Overview\t Day1 Day2 Day3 Day4 Day5\n");
	printf("\n     Week 1\t ");
	for(int i = 0; i <DAYS; i++)
	{
		printf("%3.2f ", storenumber[i]);
	}
	printf("\n     Week 2\t ");
	for(int j = DAYS; j <REST_OF_WEEK; j++)
	{
		printf("%3.2f ",storenumber[j]);
	}
	for(int k = 0; k<(WEEKS*DAYS); k++)
	{
		weektime += storenumber[k];
	}
	printf("\n");
	printf("\n     Period time:%3.2f(%3.2f)", weektime, weektime - ((float)TWO_WEEKS));
}

int find_values(char buffer[], int leave_of_absence)
{
	char temp, c;
	for(int i = 0; i<strlen(buffer); i++)
	{
		temp = buffer[i];
		if(temp == '\n' && temp == c)
		{
			//printf("\nDay of absence found at: %d",(i/SIZE_OF_LINE)+1);
			leave_of_absence = ((i/SIZE_OF_LINE)+1);
		}
		c = temp;
	}
	return leave_of_absence;
}

int find_values_2(char buffer[], int half_day, int line_size_counter)
{
	char temp;
	int counter=0, temp2 = 0;
	for(int i = 0; i<strlen(buffer); i++)
	{
		counter++;
		temp = buffer[i];
		if(temp == '\n')
		{
			if(counter > TWO && counter <OVER_HALF_OF_A_LINE)
			{
				//printf("\nFound unusual break at: %d",i/line_size_counter);
				half_day = (i / line_size_counter);
				temp2 = ((i%line_size_counter)/TIMES);
				//printf("\nAt i = %d thus at stamp: %d", i,temp2);
			}
			counter = 0;
		}
	}
	return half_day;
}

void assign_to_modified_int_buffer(int multibuffer[WEEKS*DAYS][STAMPS][TIMES], int intbuffer[], int leave_of_absence, int half_day, int stamp)
{
	{
		int l =0;
			for(int j = 0; j < (WEEKS*DAYS); j++)
				if(j == (leave_of_absence-1))
				{
					for(int k = 0; k<STAMPS;k++)
						for(int m =0; m <TIMES;m++)
						{
							multibuffer[j][k][m] = 0;
						}
				}
				else if( j == half_day+1)
				{
					for(int k = 0; k < STAMPS; k++ )
						if(k == (stamp -1) || k == (stamp))
						{
							for(int m = 0; m<TIMES; m++)
							{
								multibuffer[j][k][m] = 0;
							}
						}
						else
						{
							for(int m= 0; m < TIMES; m++)
							{
								multibuffer[j][k][m] = intbuffer[l];
								l++;
							}
						}
				}
				else
				{
					for(int k = 0; k < STAMPS; k++ )
						for(int m= 0; m < TIMES; m++)
						{
							multibuffer[j][k][m] = intbuffer[l];
							l++;
						}
				}

}
}

int find_values_3(char buffer[], int line_size_counter)
{
	char temp;
	int counter=0, stamp = 0;
	for(int i = 0; i<strlen(buffer); i++)
	{
		counter++;
		temp = buffer[i];
		if(temp == '\n')
		{
			if(counter > TWO && counter <OVER_HALF_OF_A_LINE)
			{
				stamp = ((i%line_size_counter)/TIMES);
			}
			counter = 0;
		}
	}
	return stamp;
}
