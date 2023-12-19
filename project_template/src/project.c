#include "project.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//---------------------------------------------------------------------------------------------------
/*
CompareTimes function:
    *   Utility function to comparing given appointments time,
        when sorting the given appointments in the calendar.
    
    *   Given function first compares months.
    *   Second days.
    *   Third times.
*/

int compareTimes(const void* a, const void* b) {
	appointment* a1 = (appointment* )a;
	appointment* b1 =  (appointment* )b;
	int c = 0;

	if(b1->month > a1->month){
		c = -1;
	}
    else{
        if(a1->month > b1->month){
            c = 1;
        }
        else{
            if(b1->day > a1->day){
                c = -1;
            }
            else{
                if(a1->day > b1->day){
                    c = 1;
                }
                else{
                    if(b1->hour > a1->hour) c = -1;
                    if(a1->hour > b1->hour) c = 1;
                }
            }
        }
    }
	return c;
};

/*
Add_appointment function:
    *   Function adds an appointment in calendar and
        if given command is not in given format, it gives an Error message.

    *   Also it gives Error message if appointment exists already given time in calendar.
*/

void Add_appointment(const char* parameters, Calendar* calendar){
    int month, day, hour;
    char description[1000];    //Because command line can be max 1000 characters, the description can not be more than 1000 characters.
    int a = sscanf(parameters, "%s %d %d %d", description, &month, &day, &hour);    // Dividing command in different parameters.


    if(a == 4){                 //Appointments installation into Calendar
        if((month <= 0) || (month > 12)){
        printf("Month cannot be less than 1 or greater than 12.\n");
        return;
        }
        if((hour < 0) || (hour > 23)){
            printf("Hour cannot be negative or greater than 23.\n");
            return;
        }
        if((day < 1) || (day > 31)){
            printf("Day cannot be less than 1 or greater than 31.\n");
            return;
        }

        for(int i = 0; i < calendar->count; i++){                //Checking if there isn't any meeting already in given time.
            if (calendar->appointments[i].month == month && calendar->appointments[i].day == day && calendar->appointments[i].hour == hour) {
                printf("The time slot %.2d.%.2d at %.2d is already allocated.", day, month, hour);            
                return;
            }
        };

        if(calendar->count > 0){
            calendar->appointments = (appointment* )realloc(calendar->appointments, (calendar->count + 1)*sizeof(appointment));
        }
        else{
            calendar->appointments = (appointment*)malloc((calendar->count + 1)*sizeof(appointment));
        }

        calendar->appointments[calendar->count].description = (char* )malloc(strlen(description) + 1);
        strcpy(calendar->appointments[calendar->count].description, description);

        calendar->appointments[calendar->count].month = month;
        calendar->appointments[calendar->count].day = day;
        calendar->appointments[calendar->count].hour = hour;
        calendar->count++;

        printf("SUCCESS\n");
    }
    else{
        printf("A should be followed by exactly 4 arguments.\n");
        return;
    }
};

/*
Delete_appointment function:
    *   Delete appointments in given calendar
        ,if there exists it. if appointment does not exists then it prints an error message.
    
    *   Function also gives Error message if the command arguments were invalid.
*/

void Delete_appointment(const char* parameters, Calendar* calendar){
    int month, day, hour;

    int a = sscanf(parameters, "%d %d %d", &month, &day, &hour);

    int foundIndex = -1;
    if(a == 3){
        if((month <= 0) || (month > 12)){
        printf("Month cannot be less than 1 or greater than 12.\n");
        return;
        }
        if((hour < 0) || (hour > 23)){
            printf("Hour cannot be negative or greater than 23.\n");
            return;
        }
        if((day < 1) || (day > 31)){
            printf("Day cannot be less than 1 or greater than 31.\n");
            return;
        }

        for(int i = 0; i < calendar->count; i++){
            if (calendar->appointments[i].month == month && calendar->appointments[i].day == day && calendar->appointments[i].hour == hour) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            printf("SUCCESS\n");

            free(calendar->appointments[foundIndex].description);
            
            for (int i = foundIndex; i < calendar->count - 1; i++) {
                calendar->appointments[i] = calendar->appointments[i + 1];
            }
            calendar->count--;
            calendar->appointments = (appointment* )realloc(calendar->appointments, calendar->count*sizeof(appointment));
        }
        else {
            printf("The time slot %.2d.%.2d at %.2d is not in the calendar.\n",  day, month, hour);
        }
    }
    else{
        printf("D should be followed by exactly 3 arguments.\n");
    }
};

/*
Print_calendar function:
    *   Is called, which prints given calendars appointments in sorted array.

    *   Uses the utility function CompareTimes.

    *   Gives an Error message if the command's parameters are invalid.
*/

void print_calendar(const char* parameters, Calendar* calendar){
    char description[1000];
    int a = sscanf(parameters, "%s", description);

    if(a == -1){
        qsort(calendar->appointments, calendar->count, sizeof(appointment), compareTimes);
        
        for(int i = 0; i < calendar->count; i++){
            printf("%s %.2d.%.2d at %.2d\n", calendar->appointments[i].description, calendar->appointments[i].day, calendar->appointments[i].month, calendar->appointments[i].hour);
        }
        printf("SUCCESS\n");
        return;
    }
    else{
        printf("L should be followed by exactly 0 arguments.\n");
        return;
    }

};

/*
Write_file function:
    *   Writes the given appointments in calendar in sorted order in given textfile.

    *   The sorted order of appointments are sorted the same way as in print_calendar function.

    *   Gives an error message if given file doesn't open correctly or the writing in given textfile failed.
*/

void Write_file(const char* parameters, Calendar* calendar){
    char file[1000];
    int a = sscanf(parameters, "%s", file);
    if(a == 1){
         FILE* f = fopen(file, "w");
        if(!f){
            fprintf(stderr, "Cannot open file invalid-test.txt for writing.\n");
            return;
            exit(EXIT_FAILURE);
        }
        
        qsort(calendar->appointments, calendar->count, sizeof(appointment), compareTimes);

        int c = -1;
        int j = 0;
        for(int i = 0; i < calendar->count; i++){
            char buffer[1100];
            sprintf(buffer,"%s %.2d.%.2d at %.2d\n", calendar->appointments[i].description, calendar->appointments[i].day, calendar->appointments[i].month, calendar->appointments[i].hour);
            c = fputs(buffer, f);
            j++;
        }

        if((c != EOF) || (j == calendar->count)){
            printf("SUCCESS\n");
        }
        else{
            fprintf(stderr, "Error: Writing in file failed\n");
        }

        fclose(f);
    }
    else{
        printf("W should be followed by exactly 1 argument.\n");
    }
};

/*
Load_file function:
    *   Loads the fiven textfiles appointments into the calendar
        but first frees the already existing appointments in calendar.

    *   Gives an error message if opening textfile failed.
*/

void Load_file(const char* parameters, Calendar* calendar){
    char file[1000];
    int a = sscanf(parameters, "%s", file);

    if(a == 1){
        FILE* f = fopen(file, "r");
        if (!f) {
            fprintf(stderr, "Cannot open file invalid-test.txt for reading.\n");
            return;
        }

        if(calendar->count > 0){
            for(int i = 0; i < calendar->count; i++){
                free(calendar->appointments[i].description);
            }
            free(calendar->appointments);
            calendar->count = 0;
        }

        char input[1000];
        while (fgets(input, sizeof(input), f)) {
            char description[1000];
            int hour, month, day;
            int d = sscanf(input, "%s %d.%d at %d", description, &day, &month, &hour);

            if(d == 4){
                if(calendar->count > 0){
                    calendar->appointments = (appointment* )realloc(calendar->appointments, (calendar->count + 1)*sizeof(appointment));
                }
                else{
                    calendar->appointments = (appointment* )malloc((calendar->count + 1)*sizeof(appointment));
                }
                
                calendar->appointments[calendar->count].description = (char* )malloc(strlen(description) + 1);
                strcpy(calendar->appointments[calendar->count].description, description);
                calendar->appointments[calendar->count].month = month;
                calendar->appointments[calendar->count].day = day;
                calendar->appointments[calendar->count].hour = hour;
                calendar->count++;
                
            }
            else{
                printf("Error: Loading file failed.\n");
                return;
            }     
        }

        fclose(f);
        printf("SUCCESS\n");
    }
    else{
        printf("O should be followed by exactly 1 argument.\n");
    }
};

/*
Exiting function:
    *   Function is called when exiting in program by pressing 'Q'.
        The function frees the gives dynamically allocated appointments in calendar.
*/

void Exiting(Calendar* calendar){

    if(calendar->count > 0){
        for(int i = 0; i < calendar->count; i++){
            free(calendar->appointments[i].description);
        }
    }

    if(calendar->count > 0){
        free(calendar->appointments);
        calendar->count = 0;
    }
    printf("SUCCESS\n");
};


//---------------------------------------------------------------------------------------------------------------

int main(void){

    Calendar calendar;

    calendar.count = 0;

    char input[1000];
    char command = 'A';         // Initialization of command. Pre-assumption is adding into the calendar.
    char parameters[1000];

    while (command != 'Q') {
        parameters[0] = '\0';
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%c %[^\n]", &command, parameters);
        

        switch (command){
            case 'A':                           //Add appointment in Calendar.
                Add_appointment(parameters, &calendar);    
                break;
            case 'D':                           //Delete appointment in Calendar.
                Delete_appointment(parameters, &calendar);
                break;
            case 'L':                         //Print appointments in Calendar.
                print_calendar(parameters, &calendar);
                break;
            case 'W':                           //Saving given appointments in Calendar in given text file.+
                Write_file(parameters, &calendar);
                break;
            case 'O':                           //Loads appointments in given text file.
                Load_file(parameters, &calendar);
                break;
            case 'Q':                          //Exiting program.
                Exiting(&calendar);
                break;
            default:                           
                printf("Invalid command %c\n", command);
                break;
        }
        
    }
     
    return 0;
};
