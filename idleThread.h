/*
 * idleThread.h
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#ifndef IDLETHREAD_H_
#define IDLETHREAD_H_

#include "system32.h"
#include "thread.h"

class idleThread: public Thread {
public:
	idleThread();
	virtual ~idleThread();
	void run() override;
	void start();
};

#endif /* IDLETHREAD_H_ */
