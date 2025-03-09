#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

#define SIZE 100
#define QUANTUM 10

struct node *task_list = NULL;

void add(char *name, int priority, int burst) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&task_list, newTask);
}

void schedule() {
    struct node *temp = task_list;
    Task *tasks[SIZE];
    int task_count = 0;

    // Extract tasks and reverse to restore insertion order
    while (temp != NULL && task_count < SIZE) {
        tasks[task_count++] = temp->task;
        temp = temp->next;
    }
    for (int i = 0; i < task_count / 2; i++) {
        Task *tempTask = tasks[i];
        tasks[i] = tasks[task_count - i - 1];
        tasks[task_count - i - 1] = tempTask;
    }

    // Group tasks by priority
    typedef struct {
        struct node *head;
        struct node *tail;
    } PriorityQueue;
    int max_priority = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i]->priority > max_priority)
            max_priority = tasks[i]->priority;
    }
    PriorityQueue *priority_queues = calloc(max_priority + 1, sizeof(PriorityQueue));

    for (int i = 0; i < task_count; i++) {
        Task *task = tasks[i];
        struct node *newNode = malloc(sizeof(struct node));
        newNode->task = task;
        newNode->next = NULL;

        int p = task->priority;
        if (priority_queues[p].head == NULL) {
            priority_queues[p].head = priority_queues[p].tail = newNode;
        } else {
            priority_queues[p].tail->next = newNode;
            priority_queues[p].tail = newNode;
        }
    }

    // Process each priority in descending order
    int current_time = 0;
    float total_waiting = 0, total_turnaround = 0, total_response = 0;

    for (int p = max_priority; p >= 1; p--) {
        PriorityQueue *pq = &priority_queues[p];
        if (pq->head == NULL) continue;

        // Convert linked list to array
        Task *current_tasks[SIZE];
        int num_tasks = 0;
        struct node *current_node = pq->head;
        while (current_node != NULL && num_tasks < SIZE) {
            current_tasks[num_tasks++] = current_node->task;
            current_node = current_node->next;
        }

        int remaining_burst[SIZE], original_burst[SIZE];
        int response_time[SIZE], completion_time[SIZE];
        for (int i = 0; i < num_tasks; i++) {
            original_burst[i] = current_tasks[i]->burst;
            remaining_burst[i] = original_burst[i];
            response_time[i] = -1;
            completion_time[i] = -1;
        }

        // RR queue
        int queue[SIZE * 2], front = 0, rear = 0;
        for (int i = 0; i < num_tasks; i++) queue[rear++] = i;

        while (front != rear) {
            int idx = queue[front];
            front = (front + 1) % SIZE;
        
            Task *task = current_tasks[idx];
        
            // If this is the only last remaining task in the queue, let it run completely.
            int exec_time = (remaining_burst[idx] > QUANTUM && (front != rear)) ? QUANTUM : remaining_burst[idx];
        
            if (response_time[idx] == -1)
                response_time[idx] = current_time;
        
            task->burst = remaining_burst[idx]; 
            run(task, exec_time);
            current_time += exec_time;
            remaining_burst[idx] -= exec_time;
            task->burst = remaining_burst[idx];
        
            if (remaining_burst[idx] > 0) {
                queue[rear] = idx;
                rear = (rear + 1) % SIZE;
            } else {
                completion_time[idx] = current_time;  // Correctly set completion time
            }
        }
        

        // Update metrics
        for (int i = 0; i < num_tasks; i++) {
            total_waiting += completion_time[i] - original_burst[i];
            total_turnaround += completion_time[i];
            total_response += response_time[i];
        }
    }

    printf("Average waiting time = %.2f\n", total_waiting / task_count);
    printf("Average turnaround time = %.2f\n", total_turnaround / task_count);
    printf("Average response time = %.2f\n", total_response / task_count);

    // Free allocated memory (optional, for completeness)
    for (int p = 1; p <= max_priority; p++) {
        struct node *current = priority_queues[p].head;
        while (current != NULL) {
            struct node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(priority_queues);
}