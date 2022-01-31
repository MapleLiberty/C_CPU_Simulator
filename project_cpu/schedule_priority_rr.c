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

	float map[taskNum];
	tmp = head;
	for(int i = taskNum-1; i >= 0; i--){
		map[i] = 1;
		tmp = tmp->next;
	}

	while (taskNum > 0) {

		// check current highest priority
		int highestP = head->task->priority;
		struct node* check = head;
		while(check != NULL){
			if(highestP < check->task->priority)
				highestP = check->task->priority;
			check = check->next;
		}

		// pick the 1st node with the highest priority in the list and if priority is equal, pick the node in the front
		check = head;
		struct node* current = head;
		int numH = 0; // number of nodes that have current highest priority
		while(check != NULL){
			if(check->task->priority == highestP){
				current = check;
				numH = numH + 1;
			}
			check = check->next;
		}

        if(current->task->burst <= QUANTUM || numH == 1){
            run(current->task, current->task->burst); 

			turnaround = time + current->task->burst;
			turnaroundT = turnaroundT + turnaround;
			if(map[(current->task->tid)-1] > 0){
				totalRespond = totalRespond + time;
				map[(current->task->tid)-1] = map[(current->task->tid)-1] - 1;
			}
			time = time + current->task->burst;
			totalWait = totalWait + (time - burstT[(current->task->tid)-1]);

            delete(&head, current->task);
			taskNum = taskNum - 1;
        }
		else {
			run(current->task, QUANTUM);
            current->task->burst = current->task->burst - QUANTUM;
			if(map[(current->task->tid)-1] > 0){
				totalRespond = totalRespond + time;
				map[(current->task->tid)-1] = map[(current->task->tid)-1] - 1;
			}
			time = time + QUANTUM;
            delete(&head, current->task);
            insert(&head, current->task);
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