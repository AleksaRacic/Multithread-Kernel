/*
 * system32.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef SYSTEM32_H_
#define SYSTEM32_H_

#define NULL 0

/*Hardversko maskiranje prekoida*/
#define lock() {asm{pushf; cli;}}
#define unlock() {asm popf}


/*Softversko maskiranje prekida*/
#define systemLock() {int_locked = 1;}
/*Ako se u medjuvremenu dogodi zahtev za obradu, obraditi ga, a ne odbaciti*/
#define systemUnlock() {int_locked = 0; if(switch_context_req) dispatch();}

class system32 {
public:
	static volatile int int_locked;
	static void dispatch();
	static void

	system32();
	virtual ~system32();

private:
	static volatile int switch_context_req;

	static void interrupt myTimer();
	static volatile void interrupt (*oldTimer)(...);
};


#endif /* SYSTEM32_H_ */
