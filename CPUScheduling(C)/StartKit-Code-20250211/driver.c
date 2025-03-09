/**
 * Driver.c
 *
 * Schedule is in the format:
 *  [name] [priority] [CPU burst]
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 #include "task.h"
 #include "list.h"
 #include "schedulers.h"
 
 #define SIZE 100
 #define MIN_PRIORITY 1
 #define MAX_PRIORITY 10
 
 int main(int argc, char *argv[]) {
     if (argc < 2) {
         printf("Invalid Tasks in Input File\n");
         return 1;
     }
 
     FILE *in = fopen(argv[1], "r");
     if (!in) {
         printf("Invalid Tasks in Input File\n");
         return 1;
     }
 
     char task[SIZE];
     char *temp;
     char *name;
     int priority, burst;
     int is_empty = 1;  // Flag to check if file is empty
 
     while (fgets(task, SIZE, in) != NULL) {
         is_empty = 0;  // File contains at least one line
 
         temp = strdup(task);
         name = strsep(&temp, ",");
         
         if (!temp) {
             printf("Invalid Tasks in Input File\n");
             fclose(in);
             return 1;
         }
         
         priority = atoi(strsep(&temp, ","));
         
         if (!temp) {
             printf("Invalid Tasks in Input File\n");
             fclose(in);
             return 1;
         }
         
         burst = atoi(strsep(&temp, ","));
 
         // Check for invalid values
         if (priority < MIN_PRIORITY || priority > MAX_PRIORITY || burst < 0) {
             printf("Invalid Tasks in Input File\n");
             fclose(in);
             return 1;
         }
 
         // Add the task to the scheduler's list
         add(name, priority, burst);
         free(temp);
     }
 
     fclose(in);
 
     // If file was empty, print error and terminate
     if (is_empty) {
         printf("Invalid Tasks in Input File\n");
         return 1;
     }
 
     // Invoke the scheduler
     schedule();
 
     return 0;
 }
 