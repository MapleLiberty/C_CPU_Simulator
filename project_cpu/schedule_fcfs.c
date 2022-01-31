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
	
	while (taskNum > 0) {
		struct node* current = head;
		while(1){
			if(current->next == NULL)
				break;
			current = current->next;
		}
		run(current->task, current->task->burst);

		turnaround = turnaround + taskWait + current->task->burst;
		totalWait = totalWait + taskWait;
		taskWait = taskWait + current->task->burst;
		totalRespond = totalRespond + taskRespond;
		taskRespond = taskRespond + current->task->burst;

		delete(&head, current->task);
		taskNum = taskNum - 1;
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