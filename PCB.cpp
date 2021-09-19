/*
 * PCB.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "kernel.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "debug.h"



int PCB::max_id = 0;
PCBList* PCB::allPCB = new PCBList();
volatile PCB* PCB::retPCB = NULL;
volatile int PCB::findId = -1;

PCB::PCB(Thread *myThread, StackSize stack_size, Time time_slice): myT(myThread),
processorTime(time_slice), my_status(CREATED), blocked_time(0), myID(max_id++) {
	systemLock();
	waitList = new PCBList();
	allPCB->push_back(this);
	time_left = time_slice; //vidjeti da li ovo treba?

	#ifdef PCBCREATEDEBUG
	printf("T: %d\n", myID); //promeniti na synchrnizedPrintf
	#endif

	if(stack_size > MAX_STACK_SIZE) stack_size = MAX_STACK_SIZE; //dodaj uslov i za min stack
	int sSize = stack_size / sizeof(unsigned);

	stack_pointer = new unsigned[sSize];
	if(stack_pointer != NULL){
		stack_pointer[--sSize] = 0x200; //PSW
			#ifndef BCC_BLOCK_IGNORE

			stack_pointer[--sSize] = FP_SEG(&(PCB::wrapper));
			stack_pointer[--sSize] = FP_OFF(&(PCB::wrapper));

			/*pri izlasku iz rutine se skidaju jos 9 registara*/
			// stack : PSWI, cs, ip, ax , bx , cx , dx , es ,ds , si , di , bp

			sSize -= 9;

			sp = FP_OFF(stack_pointer + sSize);
			ss = FP_SEG(stack_pointer + sSize);
			#endif
			bp = sp;
	}else{
		printf("MEMFULL: stack"); //ovo doraditi i istestirati
	}

	#ifndef BCC_BLOCK_IGNORE
	systemUnlock();
	#endif
}

//dodati static konstruktor za main

PCB::PCB() : myT(NULL), processorTime(0), my_status(CREATED), blocked_time(0), myID(max_id++) {
	bp = 0;
	ss = 0;
	sp = 0;
	//allPCB->push_back(this); videti da li ovo ima smisla
	resetMyTime();
	time_left = 0;
	stack_pointer = NULL;
	int sSize = defaultStackSize / sizeof(unsigned);
	stack_pointer = new unsigned[sSize]; //ovo je suvisno
}

PCB::~PCB() {
	waitToComplete();
	#ifndef BCC_BLOCK_IGNORE
	systemLock();
	#endif
	delete[] stack_pointer;
	allPCB->remove(this);
	myT->myPCB = NULL;
	#ifndef BCC_BLOCK_IGNORE
	systemUnlock();
	#endif

}

void PCB::waitToComplete(){
	#ifndef BCC_BLOCK_IGNORE
	lock();
	#endif

	if(this == Kernel::running || this == Kernel::idle){
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

	Kernel::running->setBlocked(0);
	waitList->push((PCB*)Kernel::running);
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	dispatch();
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


int PCB::resetBlockedWrapper(PCB* pcb){
	#ifdef UNBLOCKEDDEBUG
	printf("PCB fin: ");
	#endif
	pcb->resetBlocked();
	return 0;
}

int PCB::compareIDWrapper(PCB* pcb){
	if(pcb->getID() == findId){
		retPCB = pcb;
		return 1;
	}
	return 0;
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
	systemLock();
	findId = _id;
	retPCB = NULL;
	allPCB->applyAll(compareIDWrapper);
	PCB* tmp = (PCB*)retPCB;
	systemUnlock(); //Ako se ne zakljuca, moze neka druga nit izmeniti retPCB
	return tmp;
}

ID PCB::getRunningId(){
	return ((PCB*)Kernel::running)->getID();
}

void PCB::resetBlocked() volatile {
	#ifdef UNBLOCKEDDEBUG
	printf("RB %d, by %d\n ", myID, Kernel::running->getID());
	#endif
	my_status = READY;
	Scheduler::put((PCB*)this);
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

int PCB::decBlockedTime()volatile{
	if(blocked_time == 0) return 0;
	if(--blocked_time <= 0){
		unblocked_by_PCB = 0;
		#ifdef UNBLOCKEDDEBUG
		printf("Sem t: ");
		#endif
		resetBlocked();
		return 1;
	}else{
		return 0;
	}
}

void PCB::setBlocked(Time t = 0)volatile{
	my_status = BLOCKED;
	blocked_time = t;
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
