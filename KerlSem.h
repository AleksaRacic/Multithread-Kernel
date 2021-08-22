/*
 * KernelSem.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef KERLSEM_H_
#define KERLSEM_H_

class KernelSem {
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;
private:
	int value;
	void dec();
};

#endif /* KERLSEM_H_ */
