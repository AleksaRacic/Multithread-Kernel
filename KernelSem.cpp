/*
 * KernelSem.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "KernelSem.h"

KernelSem::KernelSem(int init): value(init) {
}

KernelSem::~KernelSem() {
	// TODO Auto-generated destructor stub
}

int KernelSem::wait(Time maxTimeToWait) {
}

void KernelSem::signal() {
}

int KernelSem::val() const {
}
