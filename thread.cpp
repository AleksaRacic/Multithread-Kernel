/*
 * thread.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "thread.h"

Thread::Thread() {
	// TODO Auto-generated constructor stub

}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void dispatch(){
	system32::dispatch(); //videti hoce li ovo praviti problem kasnje
}

Thread*  Thread::getThreadById(ID id){
	return PCB::getPCBById(id)->getThread();
}
