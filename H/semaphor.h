#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

#include "KerSem.h"

typedef unsigned int Time;

class Semaphore {
	public:
		Semaphore (int init=1);
		virtual ~Semaphore ();
		virtual int wait (Time maxTimeToWait);
		virtual void signal();
		int val () const;
	private:
		KernelSem* myImpl;
};

#endif /* SEMAPHOR_H_ */
