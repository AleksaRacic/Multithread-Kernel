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
class PCB {
public:
	static volatile ID max_id;

	enum status{NEW, READY, BLOCKED, RUNNING, FINISHED};
	PCB(Thread *myThread, StackSize stack_size, Time time_slice);
	virtual ~PCB();
	status my_status;

	unsigned *stack_pointer;
	volatile unsigned int sp, ss;
	Time processorTime;
	volatile Time time_left;

	Time blocked_time;

	ID getID()const;

	static void wrapper();
	static void start();
	void waitToComplete();
	void setLocked();
private:
	ID myID;
	Thread *myT;
	LinkedList<PCB*>* waitList;

};

#endif /* PCB_H_ */
