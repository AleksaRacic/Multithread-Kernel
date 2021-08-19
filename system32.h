/*
 * system32.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef SYSTEM32_H_
#define SYSTEM32_H_

#include "PCB.h"

#define NULL 0

/*Hardversko maskiranje prekoida*/
#define lock() {asm{pushf; cli;}}
#define unlock() {asm popf}


/*Softversko maskiranje prekida*/
#define systemLock() {system32::int_locked = 1;}
/*Ako se u medjuvremenu dogodi zahtev za obradu, obraditi ga, a ne odbaciti*/
#define systemUnlock() {system32::int_locked = 0; if(system32::getSwitchContextReq()) system32::dispatch();}

class system32 {
public:
	static volatile int int_locked;
	static void dispatch();
	static void boot();
	static void restore();

	system32();
	virtual ~system32();

	static volatile int getSwitchContextReq() const;

private:
	static volatile int switch_context_req;

	static void interrupt myTimer();
	static volatile void interrupt (*oldTimer)(...);
};


#endif /* SYSTEM32_H_ */
