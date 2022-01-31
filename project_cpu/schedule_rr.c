#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node* head = NULL;
int id = 1;
int taskNum = 0;

// add a task to the list 
void add(char* name, int priority, int burst) {
	Task* curTask = malloc(sizeof(Task));
	curTask->name = name;
	curTask->priority = priority;
	curTask->burst = burst;
	curTask->tid = id;
	id = id + 1;

	if (head == NULL) {
		head = malloc(sizeof(struct node));
		head->task = curTask;
		head->next = NULL;
	}
	else
		insert(&head, curTask); 

	taskNum = taskNum + 1;
}

// invoke the scheduler
void schedule() {
	double turnaround = 0.0;
	double turnaroundT = 0.0;
	double totalWait = 0.0;
	double totalRespond = 0.0;
	double time = 0.0;
	int taskNum2 = taskNum;

	float burstT[taskNum];
	struct node* tmp = head;
	for(int i = taskNum-1; i >= 0; i--){
		burstT[i] = tmp->task->burst;
		tmp = tmp->next;
	}

	int first = taskNum;

	while (taskNum > 0) {

		// pick the 1st node in the list
		struct node* current = head;
		while(1){
			if(current->next == NULL)
				break;
			current = current->next;
		}

        if(current->task->burst > QUANTUM){
            run(current->task, QUANTUM);
            current->task->burst = current->task->burst - QUANTUM;
			if(first > 0)
				totalRespond = totalRespond + time;
			time = time + QUANTUM;
            delete(&head, current->task);
            insert(&head, current->task);
		}
        else{
            run(current->task, current->task->burst); 

			turnaround = time + current->task->burst;
			turnaroundT = turnaroundT + turnaround;
			if(first > 0)
				totalRespond = totalRespond + time;
			time = time + current->task->burst;
			totalWait = totalWait + (time - burstT[(current->task->tid)-1]);

            delete(&head, current->task);
			taskNum = taskNum - 1;
        }

		first = first - 1;

	}

	// print waiting, turnaround, response time
	turnaroundT = turnaroundT / taskNum2;
	totalWait = totalWait / taskNum2;
	totalRespond = totalRespond / taskNum2;
	printf("\n");
	printf("Average waiting time = %.2f\n", totalWait);
	printf("Average turnaround time = %.2f\n", turnaroundT);
	printf("Average response time = %.2f\n", totalRespond);
}