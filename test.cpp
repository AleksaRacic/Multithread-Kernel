/*
 * test.cpp
 *
 *  Created on: Aug 22, 2021
 *      Author: OS1
 */

extern int synchronizedPrintf(const char *format,...);

void tick(){
	synchronizedPrintf("tick");
}

int userMain(int argc, char **argv){
	for(int i = -6; i<600; i++){
		for(int i = -6; i<500; i++){

			}
		tick();
	}
}



