#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *task_list = NULL;

// Function to add tasks to the task list
void add(char *name, int priority, int burst) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&task_list, newTask);
}

// Function to select the next task with the shortest burst time, preferring the earliest added (FIFO for ties)
struct node* pickNextTaskSJF(struct node **head) {
    struct node *temp = *head;
    struct node *shortestTask = NULL;
    int min_burst = -1;

    while (temp != NULL) {
        if (shortestTask == NULL || temp->task->burst < min_burst || temp->task->burst == min_burst) {
            min_burst = temp->task->burst;
            shortestTask = temp;
        }
        temp = temp->next;
    }

    return shortestTask;
}

// SJF scheduling function
void schedule() {
    struct node *temp;
    int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0, current_time = 0;
    int num_tasks = 0;

    while (task_list != NULL) {
        num_tasks++;
        temp = pickNextTaskSJF(&task_list); // Select the task with the shortest burst time (FIFO for ties)
        run(temp->task, temp->task->burst); // Execute the task
        
        total_waiting_time += current_time;
        total_response_time += current_time; // Response time = waiting time in non-preemptive SJF
        current_time += temp->task->burst;
        total_turnaround_time += current_time;

        delete(&task_list, temp->task); // Remove task after execution
    }

    printf("Average waiting time = %.2f\n", (float)total_waiting_time / num_tasks);
    printf("Average turnaround time = %.2f\n", (float)total_turnaround_time / num_tasks);
    printf("Average response time = %.2f\n", (float)total_response_time / num_tasks);
}