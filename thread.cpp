/*
 * thread.cpp
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include "kernel.h"

Thread::Thread(StackSize s, Time ts) {
	myPCB = new PCB(this, s, ts);
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

void dispatch(){
	Kernel::dispatch(); //videti hoce li ovo praviti problem kasnje
}

Thread*  Thread::getThreadById(ID id){
	return PCB::getPCBById(id)->getThread();
}

void Thread::start(){
	myPCB->start();
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
