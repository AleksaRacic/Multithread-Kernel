/*
 * PCB.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "kernel.h"
#include "PCB.h"
#include "SCHEDULE.H"


#include <STDIO.H>


int PCB::max_id = 0;
PCBList* PCB::allPCB = new PCBList();

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
processorTime(time_slice), my_status(CREATED), blocked_time(0), myID(max_id++) {
	systemLock();
	waitList = new PCBList();
	allPCB->push_back(this);
	time_left = time_slice; //vidjeti da li ovo treba?
	//printf("S %d\n", allPCB->size);
	//printf("\n");
	printf("T: %d\n", myID);

	if(stack_size > MAX_STACK_SIZE) stack_size = MAX_STACK_SIZE;
	int sSize = stack_size / sizeof(unsigned);

	stack_pointer = new unsigned[sSize];

	// stack : PSWI, cs, ip, ax , bx , cx , dx , es ,ds , si , di , bp

	stack_pointer[--sSize] = 0x200; //PSW
	#ifndef BCC_BLOCK_IGNORE

	stack_pointer[--sSize] = FP_SEG(&(PCB::wrapper));
	stack_pointer[--sSize] = FP_OFF(&(PCB::wrapper));

	/*pri izlasku iz rutine se skidaju jos 9 registara*/
	sSize -= 9;

	sp = FP_OFF(stack_pointer + sSize);
	ss = FP_SEG(stack_pointer + sSize);
	bp = sp;
	#endif

	#ifndef BCC_BLOCK_IGNORE
	systemUnlock();
	#endif
}

PCB::PCB() : myT(NULL), processorTime(0), my_status(CREATED), blocked_time(0), myID(max_id++) {
	bp = 0;
	ss = 0;
	sp = 0;

	//allPCB->push_back(this); videti da li ovo ima smisla
	resetMyTime();
	time_left = 0;
	int sSize = defaultStackSize / sizeof(unsigned);
	stack_pointer = new unsigned[sSize];
}

PCB::~PCB() {
	waitToComplete();
	#ifndef BCC_BLOCK_IGNORE
	lock();
	delete[] stack_pointer;
	unlock();
	#endif
	//dodati da se izbaci pcb iz liste
	//dodati da myPCB od threda bude NULL

}

void PCB::setBlocked()volatile{
	my_status = BLOCKED; //ovdje puca
}

void PCB::waitToComplete(){
	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif

	if(this == Kernel::running){ //dodati || this == idle
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

	Kernel::running->setBlocked();
	waitList->push((PCB*)Kernel::running); //vidjeti zasto push_back ne radi?
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	dispatch();
}


void PCB::resetBlocked(){
	printf("RB %d, by %d", myID, Kernel::running->getID());
	my_status = READY;
	Scheduler::put(this);
}

void PCB::setFinished()volatile{
	my_status = FINISHED;
}

void PCB::start(){
	#ifndef BCC_BLOCK_IGNORE
	lock()
	#endif
	if(my_status == CREATED){
		my_status = READY;
		Scheduler::put(this);
	}
	#ifndef BCC_BLOCK_IGNORE
	unlock()
	#endif
}

void PCB::run()volatile{
	myT->run();
}

PCBList* PCB::getWaitList(){
	return waitList;
}

void PCB::unblockWaitList()volatile{
	/*Unsynchronized function*/
	waitList->applyAll(resetBlockedWrapper);
}


void PCB::resetBlockedWrapper(PCB* pcb){
	pcb->resetBlocked();
}

void PCB::wrapper(){
	Kernel::running->run();
	#ifndef BCC_BLOCK_IGNORE
	lock(); //videti koji unblock ide ovde
	#endif
	Kernel::running->setFinished();
	Kernel::running->unblockWaitList();
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	Kernel::dispatch();
}


ID PCB::getID()volatile{
	return myID;
}

PCB* PCB::getPCBById(ID _id){
	return allPCB->findID(_id);

}



ID PCB::getRunningId(){
	return ((PCB*)Kernel::running)->getID();
}

int PCB::decProcessorTime()volatile{
	if(processorTime == 0) return 0;
	if(--time_left <= 0){
		my_status = READY; // ovo nije dobro, ja mislim
		return 1;
	}else{
		return 0;
	}
}

Status PCB::getStatus()volatile{
	return my_status;
}

void PCB::resetMyTime()volatile{
	time_left = processorTime;
	my_status = RUNNING;
}

Thread* PCB::getThread(){
	return myT;
}
