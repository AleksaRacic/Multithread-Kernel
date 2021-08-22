/*
 * system32.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "system32.h"

volatile void interrupt (*system32::oldTimer)(...) = NULL;
volatile int system32::switch_context_req = 0;

system32::system32() {

}

system32::~system32() {
	// TODO Auto-generated destructor stub
}

void system32::dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	switch_context_req = 1;
	myTimer();
	switch_context_req = 0;
#ifndef BCC_BLOCK_IGNORE
	//unlock();
#endif

}

int system32::getSwitchContextReq() {
	return switch_context_req;
}

void interrupt system32::myTimer(){

	int time_left;

	if(!switch_context_req){
		tick();
		(*oldTimer)();
		time_left = running->decProcessorTime();

		//Ovde dodati da se Odblokiraju semafori kojima je isteklo vreme

		if(!time_left) changeContext();
	}else{
		//videti da li ova logika prolazi (da li se moze dogoditi ugnezdeni interrupti)
		changeContext();
	}
}

void system32::changeContext(){
	unsigned int tsp, tss;
	PCB *next_thread = NULL;
	if(running != idle && running->getStatus() == PCB::READY){
		Scheduler::put(running);
	}
	next_thread = Scheduler::get();
	if(next_thread == NULL) next_thread = idle;

	#ifndef BCC_BLOCK_IGNORE
	asm{
		mov tsp, sp
		mov tss, ss
	}
	#endif
	//Skontati sta se desava sa bp i ostalim registrima
	running->sp = tsp;
	running->ss = tss;

	running = next_thread;
	running->resetMyTime();

	tsp = running->sp;
	tss = running->ss;

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov sp, tsp
		mov ss, tss
	}
	#endif

}

void system32::boot(){
#ifndef BCC_BLOCK_IGNORE
	lock(); //zaustavljamo sve interrupte dok se sistem pali
	oldTimer = getvect(0x08);
	setvect(0x08, myTimer);
#endif
	idleT = new idleThread();
	idleT->start();
	idle = idleT->myPCB;

	mainPCB = new PCB(NULL); //videti da li ce zbog ovoga pucati(ne bi trebalo jer se ne poziva run)
	running = mainPCB;
	//dodati semafore i ostalo

#ifndef BCC_BLOCK_IGNORE

	unlock();
#endif
}

void system32::restore(){
	#ifndef BCC_BLOCK_IGNORE
		lock(); //zaustavljamo sve interrupte dok se sistem pali
		setvect(0x08, oldTimer);
	#endif
		delete idleT;
		delete mainPCB;
	#ifndef BCC_BLOCK_IGNORE
		unlock(); //zaustavljamo sve interrupte dok se sistem pali
	#endif
}



