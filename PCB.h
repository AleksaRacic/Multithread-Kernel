/*
 * PCB.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"
#include "SCHEDULE.H"
#include "system32.h"

#define MAX_STACK_SIZE 65534 //64KB - 1

class PCB {
public:
	static volatile int max_id;
	PCB(Thread *myThread, StackSize stack_size, Time time_slice);
	enum status {CREATED, READY, BLOCKED, RUNNING, FINISHED};

	virtual ~PCB();
	status my_status;

	static PCB* getPCBById(int);

	static int getRunningId();

	unsigned *stack_pointer;
	volatile unsigned int sp, ss;
	Time processorTime;

	volatile Time time_left;

	volatile Time blocked_time;

	int getID()const;

	static LinkedList<PCB*>* allPCB;

	static void wrapper();
	static void start();
	void waitToComplete();
	void setBlocked();
	void resetBlocked();
	void run();
	void setFinished();
	void unblockWaitList();
	status getStatus();

	static int compareIDWrapper(PCB*, int);
	static void resetBlockedWrapper(PCB*);

	Thread* getThread();

	int decProcessorTime();
	int decBlockedTime();

	void resetMyTime();

private:
	LinkedList<PCB*>* getWaitList()const;
	int myID;
	Thread *myT;
	LinkedList<PCB*>* waitList;

};

#endif /* PCB_H_ */
