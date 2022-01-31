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
	double taskWait = 0.0;
	double totalWait = 0.0;
	double taskRespond = 0.0;
	double totalRespond = 0.0;
	int taskNum2 = taskNum;

	Task* highestTask;

	while(head != NULL){

		// pick the node with the highest priority and if priority is equal, pick the node with the front tid
		Task* curTask = head->task;
		struct node* nextNode = head->next;
		while (nextNode != NULL) {
			if(nextNode->task->priority > curTask->priority)
				curTask = nextNode->task;
			else if(nextNode->task->priority == curTask->priority)
				if(nextNode->task->tid < curTask->tid)
					curTask = nextNode->task;
			nextNode = nextNode->next;
		}

		highestTask = curTask;
		run(highestTask, highestTask->burst);

		turnaround = turnaround + taskWait + highestTask->burst;
		totalWait = totalWait + taskWait;
		taskWait = taskWait + highestTask->burst;
		totalRespond = totalRespond + taskRespond;
		taskRespond = taskRespond + highestTask->burst;
		
		delete(&head, highestTask);
	}

	// print waiting, turnaround, response time
	turnaround = turnaround / taskNum2;
	totalWait = totalWait / taskNum2;
	totalRespond = totalRespond / taskNum2;
	printf("\n");
	printf("Average waiting time = %.2f\n", totalWait);
	printf("Average turnaround time = %.2f\n", turnaround);
	printf("Average response time = %.2f\n", totalRespond);
}