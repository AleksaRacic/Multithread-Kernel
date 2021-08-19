/*
 * idleThread.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include "idleThread.h"

idleThread::idleThread() : Thread(128,1){

}

idleThread::~idleThread() {
	// TODO Auto-generated destructor stub
}

void idleThread::run(){
	while(1);
}

void idleThread::start(){

}
