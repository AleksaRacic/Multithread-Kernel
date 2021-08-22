/*
 * PCB.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "PCB.h"

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
processorTime(time_slice), my_status(CREATED), blocked_time(0), myID(max_id++) {
	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif
	waitList = new LinkedList<PCB*>;

	if(stack_size > MAX_STACK_SIZE) stack_size = MAX_STACK_SIZE;
	int sSize = stack_size / sizeof(unsigned);

	stack_pointer = new unsigned[sSize];

	stack_pointer[--sSize] = 0x200; //PSW
	#ifndef BCC_BLOCK_IGNORE

	stack_pointer[--sSize] = FP_SEG(wrapper);
	stack_pointer[--sSize] = FP_OFF(wrapper);

	/*pri izlasku iz rutine se skidaju jos 9 registara*/
	sSize -= 9;


	sp = FP_OFF(stack_pointer + sSize);
	ss = FP_SEG(stack_pointer + sSize);
	//bp = FP_OFF(stack_pointer + sSize);
	#endif
	allPCB->push_back(this);
	#ifndef BCC_BLOCK_IGNORE
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
		unlock();
		return;
		#endif
	}

	if(my_status == CREATED || my_status == FINISHED){
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		return;
		#endif
	}

	system32::running->setBlocked();
	waitList->push_back(system32::running);
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	dispatch();
}

void PCB::setBlocked(){
	my_status = BLOCKED;
}

void PCB::resetBlocked(){
	my_status = READY;
	Scheduler::put(this);
}

void PCB::setFinished(){
	my_status = FINISHED;
}

void PCB::start(){
	#ifndef BCC_BLOCK_IGNORE
	lock()
	if(my_status == CREATED){
		my_status = READY;
		Scheduler::put(this);
	}
	unlock()
	#endif
}

void PCB::run(){
	myT->run;
}

LinkedList<PCB*>* PCB::getWaitList()const{
	return waitList;
}

void PCB::unblockWaitList(){
	/*Unsynchronized function*/
	waitList->applyAll(resetBlockedWrapper);
}

void PCB::resetBlockedWrapper(PCB* pcb){
	pcb->resetBlocked();
}

void PCB::wrapper(){
	system32::running->run();
	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif
	system32::running->setFinished();
	system32::running->unblockWaitList();
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	system32::dispatch();
}

int compareIDWrapper(PCB* pcb, int _id){
	return 1 ? pcb->getID() == _id : 0;
}

ID PCB::getID()const{
	return myID;
}

PCB* PCB::getPCBById(ID _id){
	return *(allPCB->findFunction(compareIDWrapper, _id));
}

int PCB::max_id = 0;

LinkedList<PCB*> PCB::allPCB = new LinkedList<PCB*>;

ID PCB::getRunningId(){
	return system32::running->getID();
}

int PCB::decProcessorTime(){
	if(processorTime == 0) return -1;
	if(--time_left <= 0){
		my_status = READY;
		return 0;
	}else{
		return -1;
	}
}

PCB::status PCB::getStatus(){
	return my_status;
}

void PCB::resetMyTime(){
	time_left = processorTime;
	my_status = RUNNING;
}

