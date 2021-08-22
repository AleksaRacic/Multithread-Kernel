/*
 * semaphore.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "KerlSem.h"

typedef unsigned int Time;


class Semaphore {
	public:
		Semaphore (int init=1);
		virtual ~Semaphore ();
		virtual int wait (Time maxTimeToWait);
		virtual void signal();
		int val () const; // Returns the current value of the semaphore
	private:
		KernelSem* myImpl;
};

#endif /* SEMAPHORE_H_ */
