/*
 * userMainThread.cpp
 *
 *  Created on: Aug 22, 2021
 *      Author: OS1
 */

#include "uMainT.h"

userMainThread::uMainT(int argc_, char** argv_):Thread(), argc(argc_), argv(argv_) {}

userMainThread::~uMainT() {
	// TODO Auto-generated destructor stub
}

void userMainThread::run() {
	ret = userMain(argc, argv);
}
