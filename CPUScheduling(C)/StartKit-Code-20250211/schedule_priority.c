#include <stdio.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *task_list = NULL;

void add(char *name, int priority, int burst) {
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&task_list, newTask);
}

// Updated to select the earliest arrived task among those with the highest priority
struct node* pickHighestPriority(struct node **head) {
    struct node *current = *head;
    struct node *selected = NULL;
    int max_priority = -1;

    while (current != NULL) {
        if (current->task->priority > max_priority) {
            max_priority = current->task->priority;
            selected = current;
        } else if (current->task->priority == max_priority) {
            // Select the later node (earlier arrival due to insertion at head)
            selected = current;
        }
        current = current->next;
    }
    return selected;
}

void schedule() {
    struct node *temp;
    int total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0, current_time = 0;
    int num_tasks = 0;

    while (task_list != NULL) {
        num_tasks++;
        temp = pickHighestPriority(&task_list);
        run(temp->task, temp->task->burst);
        
        total_waiting_time += current_time;
        total_response_time += current_time;
        current_time += temp->task->burst;
        total_turnaround_time += current_time;

        delete(&task_list, temp->task);
    }

    printf("Average waiting time = %.2f\n", (float)total_waiting_time / num_tasks);
    printf("Average turnaround time = %.2f\n", (float)total_turnaround_time / num_tasks);
    printf("Average response time = %.2f\n", (float)total_response_time / num_tasks);
}