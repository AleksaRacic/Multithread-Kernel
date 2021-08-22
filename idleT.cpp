/*
 * idleThread.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include "idleT.h"

idleThread::idleThread() : Thread(512,1){
	running = 1;
}

idleThread::~idleThread() {
	running = 0;
	//Proveriti da li se implicitno poziva wait to complete
}

void idleThread::run(){
	while(running);
}



