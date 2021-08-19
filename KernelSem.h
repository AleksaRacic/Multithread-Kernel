/*
 * KernelSem.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef KERNELSEM_H_
#define KERNELSEM_H_

class KernelSem {
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;
private:
	int value;

};

#endif /* KERNELSEM_H_ */
