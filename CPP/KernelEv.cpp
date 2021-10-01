#include "KernelEv.h"

#include "KernelEv.h"
#include "kernel.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"
#include "PCB.h"

extern IVTEntry* IVTEntries[];

KernelEv::KernelEv(IVTNo ivtNo) : entryNum(ivtNo) {
	systemLock();
	myPCB = (PCB*)Kernel::running;
	IVTEntries[ivtNo]->setKernelEv(this);
	systemUnlock();
}

KernelEv::~KernelEv() {
	IVTEntries[entryNum]->setKernelEv(NULL);
}

void KernelEv::wait() {
	if(myPCB != (PCB*)Kernel::running){
		systemUnlock();
		return;
	}
	myPCB->setBlocked(0);
	systemUnlock();
	Kernel::dispatch();

}

void KernelEv::signal() {
	if(myPCB->my_status == BLOCKED){
		#ifdef UNBLOCKEDDEBUG
		printf("Ev: ");
		#endif
		myPCB->resetBlocked();
	}
}






