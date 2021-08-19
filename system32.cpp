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
	// TODO Auto-generated constructor stub

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
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}

static volatile int system32::getSwitchContextReq()const {
	return switch_context_req;
}

void interrupt system32::myTimer(){
	static PCB *next_thread = NULL;
	static volatile unsigned int tsp, tss;

	if(!switch_context_req){
		//tick();
		(*oldTimer)();
	}
	if(!switch_context_req){
		//smanjiti svima vreme cekanja kod semafora
	}
	switch_context_req = 0;
}

