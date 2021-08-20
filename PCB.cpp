/*
 * PCB.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "PCB.h"

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
processorTime(time_slice), my_status(NEW), blocked_time(0), myID(max_id++) {

	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif
	int sSize = stack_size / sizeof(unsigned);
	stack_pointer = new unsigned[sSize];

	stack_pointer[--sSize] = 0x200; //PSW
	#ifndef BCC_BLOCK_IGNORE

	stack_pointer[--sSize] = FP_SEG(wrapper);
	stack_pointer[--sSize] = FP_OFF(wrapper);

	/*pri izlasku iz rutine se skidaju jos 9 registara*/
	sSize -=9;


	sp = FP_OFF(stack_pointer + sSize);
	ss = FP_SEG(stack_pointer + sSize);
	//bp = FP_OFF(stack_pointer + sSize);
	unlock();
	#endif


}

PCB::~PCB() {
	waitToComplete();
	#ifndef BCC_BLOCK_IGNORE
	lock();
	delete[] stack_pointer;
	unlock();
	#endif

}

void PCB::waitToComplete(){
	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif

	if(this == system32::running || this == system32::idle){
		#ifndef BCC_BLOCK_IGNORE
		lock();
		return;
		#endif
	}

	if(my_status == NEW || my_status == FINISHED){
		#ifndef BCC_BLOCK_IGNORE
		lock();
		return;
		#endif
	}

	system32::running->setLocked();
	waitList->push_back(system32::running);
	dispatch();
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
}

void PCB::setLocked(){
	my_status = BLOCKED;
}


volatile int PCB::max_id = 0;
