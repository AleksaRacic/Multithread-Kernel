#include "KerSem.h"
#include "PCB.h"
#include "kernel.h"
#include "SCHEDULE.H"
#include "debug.h"


SemList* KernelSem::allSem = new SemList();

KernelSem::KernelSem(int init): value(init) {
	blockedPCB = new PCBList();
	allSem->push_back(this);
}

KernelSem::~KernelSem() {
	allSem->remove(this);
	delete blockedPCB;
}

int KernelSem::block(Time maxTimeToWait){
	Kernel::running->setBlocked(maxTimeToWait);

	#ifdef UNBLOCKEDDEBUG
	synchronizedPrintf("Sem blocked %d by: %d\n", Thread::getRunningId() ,maxTimeToWait);
	#endif

	blockedPCB->push_back((PCB*)Kernel::running);
	systemUnlock();
	dispatch();
	return Kernel::running->unblocked_by_PCB;
}

void KernelSem::unblock(){
	PCB* pcb = blockedPCB->first();
	blockedPCB->remove(pcb);
	pcb->unblocked_by_PCB = 1;

	#ifdef UNBLOCKEDDEBUG
	synchronizedPrintf("Sem pcb: ");
	#endif

	pcb->resetBlocked();
}

int KernelSem::wait(Time maxTimeToWait) {
	if(value-- <= 0) return this->block(maxTimeToWait);

	#ifdef UNBLOCKEDDEBUG
	synchronizedPrintf("Sem pass: %d\n", value);
	#endif

	return 2;
}

void KernelSem::signal() {
	if(value++ < 0) unblock();
}

int KernelSem::val() const {
	return value;
}

int KernelSem::decTimeWrapper(PCB* pcb){
	pcb->decBlockedTime();
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
