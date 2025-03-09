#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

//max number of tasks defined by driver.c
#define SIZE 100

struct node *task_list = NULL;

void add(char *name, int priority, int burst) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    
    // Insert tasks using the existing list function
    insert(&task_list, newTask);
}

void schedule() {
    struct node *temp = task_list;
    Task *tasks[SIZE]; // Array to store tasks
    int task_count = 0;

    // Step 1: Store tasks in an array
    while (temp != NULL && task_count < SIZE) {
        tasks[task_count++] = temp->task;
        temp = temp->next;
    }

    // Step 2: Reverse the array to maintain FIFO order
    for (int i = 0; i < task_count / 2; i++) {
        Task *tempTask = tasks[i];
        tasks[i] = tasks[task_count - i - 1];
        tasks[task_count - i - 1] = tempTask;
    }

    // Step 3: Execute tasks in FCFS order
    int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0, current_time = 0;

    for (int i = 0; i < task_count; i++) {
        Task *task = tasks[i];
        run(task, task->burst);  // Execute the task

        total_waiting_time += current_time;
        total_response_time += current_time; // Response time = waiting time in FCFS
        current_time += task->burst;
        total_turnaround_time += current_time;
    }

    // Step 4: Print final results
    printf("Average waiting time = %.2f\n", (float)total_waiting_time / task_count);
    printf("Average turnaround time = %.2f\n", (float)total_turnaround_time / task_count);
    printf("Average response time = %.2f\n", (float)total_response_time / task_count);
}