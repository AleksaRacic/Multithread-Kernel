/*
 * KernelSem.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "KerSem.h"
#include "kernel.h"
#include "debug.h"

extern int synchronizedPrintf(const char *format,...);

SemList* KernelSem::allSem = new SemList();

KernelSem::KernelSem(int init): value(init) {
	blockedPCB = new PCBList();
	allSem->push_back(this);
}

KernelSem::~KernelSem() {
	//ne sme nestati ako i dalje ima PCBova na njemu, ili sme. Skontati
	allSem->remove(this);
	delete blockedPCB;
}

int KernelSem::block(Time maxTimeToWait){
	Kernel::running->setBlocked(maxTimeToWait);
	blockedPCB->push_back((PCB*)Kernel::running);
	//Videti da li treba informacija o tome na kojem semaforu je blokiran pcb
	dispatch();
	return Kernel::running->unblocked_by_PCB;
}

void KernelSem::unblock(){
	PCB* pcb = blockedPCB->first();
	blockedPCB->remove(pcb);  //spojiti ovo u jednu funnkciju unutar liste
	pcb->unblocked_by_PCB = 1;
	#ifdef UNBLOCKEDDEBUG
	printf("Sem pcb: ");
	#endif
	pcb->resetBlocked();
}

int KernelSem::wait(Time maxTimeToWait) {
	if(value-- <= 0) return this->block(maxTimeToWait);
	return 0;
}

void KernelSem::signal() {
	if(value++ < 0) unblock();
}

int KernelSem::val() const {
	return value;
}

int KernelSem::decTimeWrapper(PCB* pcb){
	pcb->decBlockedTime();
	//synchronizedPrintf("T:%d tl %d\n", pcb->getID(), pcb->blocked_time);
	return 0;
}

void KernelSem::decTime(){
	blockedPCB->applyAll(decTimeWrapper);
}

int KernelSem::decAllWrapper(KernelSem* KS){
	KS->decTime();
	return 0;
}

void KernelSem::decAllSem(){
	allSem->applyAll(decAllWrapper);
}
