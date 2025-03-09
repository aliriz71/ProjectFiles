#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

#define SIZE 100
#define QUANTUM 10

struct node *task_list = NULL;

// Original add() uses insert() to add tasks to the head 
void add(char *name, int priority, int burst) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&task_list, newTask); // Inserts at head (reversed order)
}

void schedule() {
    struct node *temp = task_list;
    Task *tasks[SIZE];
    int task_count = 0;

    // Extract tasks in reversed order (as stored in the linked list)
    while (temp != NULL && task_count < SIZE) {
        tasks[task_count++] = temp->task;
        temp = temp->next;
    }

    // Reverse the array to restore original insertion order
    for (int i = 0; i < task_count / 2; i++) {
        Task *tempTask = tasks[i];
        tasks[i] = tasks[task_count - i - 1];
        tasks[task_count - i - 1] = tempTask;
    }

    // Initialize RR variables
    int remaining_burst[SIZE];
    int original_burst[SIZE];
    int completion_time[SIZE] = {0};
    int response_time[SIZE] = {0};
    int queue[SIZE];
    int front = 0, rear = 0;
    int current_time = 0;

    // Initialize task tracking
    for (int i = 0; i < task_count; i++) {
        remaining_burst[i] = tasks[i]->burst;
        original_burst[i] = tasks[i]->burst; // Preserve original burst
        queue[rear++] = i; // Enqueue all tasks
    }

    // Process tasks in RR order
    while (front != rear) {
        int idx = queue[front];
        front = (front + 1) % SIZE;

        Task *task = tasks[idx];
        int exec_time = (remaining_burst[idx] > QUANTUM) ? QUANTUM : remaining_burst[idx];

        // Update task's burst for display
        task->burst = remaining_burst[idx];

        // Track response time (first execution)
        if (response_time[idx] == 0 && remaining_burst[idx] == original_burst[idx]) {
            response_time[idx] = current_time;
        }

        run(task, exec_time); // Show updated burst when printed
        current_time += exec_time;
        remaining_burst[idx] -= exec_time;

        // Update task's burst again for next display
        task->burst = remaining_burst[idx];

        // Re-enqueue if not finished
        if (remaining_burst[idx] > 0) {
            queue[rear] = idx;
            rear = (rear + 1) % SIZE;
        } else {
            completion_time[idx] = current_time;
        }
    }

    // Calculate metrics
    float total_waiting = 0, total_turnaround = 0, total_response = 0;
    for (int i = 0; i < task_count; i++) {
        total_turnaround += completion_time[i];
        total_waiting += completion_time[i] - original_burst[i];
        total_response += response_time[i];
    }

    printf("Average waiting time = %.2f\n", total_waiting / task_count);
    printf("Average turnaround time = %.2f\n", total_turnaround / task_count);
    printf("Average response time = %.2f\n", total_response / task_count);
}