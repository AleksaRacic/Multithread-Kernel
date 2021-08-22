/*
 * idleThread.h
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#ifndef IDLET_H_
#define IDLET_H_

#include "system32.h"
#include "thread.h"

class idleThread : public Thread {
public:
	idleThread();
	virtual ~idleThread();
	int running;
};

#endif /* IDLET_H_ */
