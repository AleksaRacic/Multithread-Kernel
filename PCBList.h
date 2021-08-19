/*
 * PCBList.h
 *
 *  Created on: Aug 18, 2021
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "PCB.h"

class PCBList {
public:
	PCBList();
	virtual ~PCBList();
	PCB *get();
	void *put();
	void decAll();
private:
	struct Node{
			PCB *pcb;
			Node *next;
			Node(PCB *p):pcb(p),next(0){};
			Node(PCB *p, Node *nxt):pcb(p),next(nxt){};
		};
	Node *first, *last;

};


#endif /* PCBLIST_H_ */
