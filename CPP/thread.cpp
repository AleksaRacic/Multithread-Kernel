#include "thread.h"
#include "PCB.h"
#include "kernel.h"
#include "debug.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(this, stackSize, timeSlice);
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

void dispatch(){
	Kernel::dispatch();
}

Thread*  Thread::getThreadById(ID id){
	return PCB::getPCBById(id)->getThread();
}

void Thread::start(){
	systemLock();
	myPCB->start();
	systemUnlock();
}

ID Thread::getRunningId(){
	return PCB::getRunningId();
}

void Thread::waitToComplete(){
	myPCB->waitToComplete();
}

ID Thread::getId(){
	return myPCB->getID();
}

void Thread::exit(){
	systemLock();
	Kernel::running->setFinished();
	systemUnlock();
	Kernel::dispatch();
}

ID Thread::fork(){
	systemLock();
	Thread* child = NULL;
	if(!(child = ((PCB*)Kernel::running)->getThread()->clone())){
		systemUnlock();
		return -1;
	}
	if(child->myPCB->clone((PCB*)Kernel::running)){ //clone kopira stack, odavde izlaze oba PCB
		systemUnlock();
		return -1;
	}
	if(child->getId() == Kernel::running->getID()){
		#ifdef FORKDEBUG
		printf("Ch run %d, ch %d \n", getRunningId(), Kernel::running->childreen_no);
		#endif
		systemUnlock();
		return 0;
	}else{
		Kernel::running->childreen_no++;
		child->myPCB->setReady();
		#ifdef FORKDEBUG
		printf("Cr ch %d by %d, %d\n", child->getId(), Kernel::running->getID(), Kernel::running->childreen_no);
		#endif
		systemUnlock();
		return child->getId();
	}
}

Thread* Thread::clone() const {
	return NULL;
}

void Thread::waitForForkChildren(){
	for(int i = 0; i < Kernel::running->childreen_no; i++){
		Kernel::running->getSemaphore()->wait(0);
	}
	Kernel::running->deleteSem();
}
