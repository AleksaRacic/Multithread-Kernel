/*
 * PCB.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "PCB.h"

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
time_left(), my_status(NEW), blocked_time(0), myID(max_id++) {

	systemLock();



}

PCB::~PCB() {
	// TODO Auto-generated destructor stub
}

volatile int PCB::max_id = 0;
