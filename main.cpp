/*
 * main.cpp
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#include "system32.h";

int main(int argc, char* argv[]){

	system32::boot();

	userMainThread user_thread(argc, argv);
	user_thread.start();
	user_thread.waitToComplete();

	system32::restore();


	//Mozda dodti kontrolu da zatvori sve niti koje se iyvrsvaju(ovo se dogadja zbog loseg koda u mainu)
	return user_thread.ret;
}

