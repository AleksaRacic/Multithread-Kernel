#include <dos.h>
#include "KernelEv.h"
#include "IVTEntry.h"
#include "kernel.h"


IVTEntry* IVTEntries[256];

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine) : entryNum(ivtNo), kernelEvent(NULL) {
	IVTEntries[ivtNo] = this;
	#ifndef BCC_BLOCK_IGNORE
	lock();
	oldRoutine = getvect(entryNum);
	setvect(entryNum, interruptRoutine);
	unlock();
	#endif
}

IVTEntry::~IVTEntry(){
	oldRoutine(); //videti da li treba block
	#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(entryNum, oldRoutine);
	unlock();
	#endif

}

//dereferencing pointer to old routine
void IVTEntry::callOldRoutine(){
	(*oldRoutine) ();
}

//delegate signal to kernelEv
void IVTEntry::signal() {
	if(kernelEvent != NULL) kernelEvent->signal();
}

//static getter
IVTEntry* IVTEntry::getEntry(IVTNo ivtNo){
	return IVTEntries[ivtNo];
}

//setter
void IVTEntry::setKernelEv(KernelEv* event){
	kernelEvent = event;
}
