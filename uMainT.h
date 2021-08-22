/*
 * userMainThread.h
 *
 *  Created on: Aug 22, 2021
 *      Author: OS1
 */

#ifndef UMAINT_H_
#define UMAINT_H_

#include "thread.h"

extern int userMain(int, char**);

class uMainT: public Thread {
public:
	uMainT(int, char**);
	virtual ~uMainT();
	volatile int ret;
private:
	int argc;
	char** argv;
};

#endif /* UMAINT_H_ */
