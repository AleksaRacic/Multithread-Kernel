/*
 * PCB.h
 *
 *  Created on: Sep 15, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#define MAX_STACK_SIZE 65534 //64KB - 1

#include "thread.h"

typedef enum {CREATED, READY, BLOCKED, RUNNING, FINISHED} Status; //mozda dodati i status idle

class PCBList;

class PCB {
public:
	static ID max_id;

	PCB(Thread*, StackSize, Time);
	PCB();

	virtual ~PCB();
	Status my_status;

	static PCB* getPCBById(int);

	static int getRunningId();

	unsigned *stack_pointer;
	unsigned int sp, ss, bp;

	Time processorTime;
	Time time_left;
	Time blocked_time;

	int getID()volatile;

	static PCBList* allPCB;

	static void wrapper();

	void start();

	void waitToComplete();
	void setBlocked() volatile;
	void resetBlocked();
	void run()volatile;
	void setFinished()volatile;
	void unblockWaitList()volatile;
	Status getStatus()volatile;

	static int compareIDWrapper(PCB*, int);
	static void resetBlockedWrapper(PCB*);

	Thread* getThread();

	int decProcessorTime()volatile;
	int decBlockedTime()volatile;

	void resetMyTime()volatile;


	friend class Kernel;
	PCBList* getWaitList();
private:
	int myID;
	Thread* myT;
	PCBList* waitList;

};

class PCBList{
public:
	class LinkedListNode {
		public:
			LinkedListNode(PCB*);
			PCB* data;
			LinkedListNode* next, * prev;
		};

	PCBList();
	~PCBList();
	void syncPrintList(); //Kasnije implementirati
	void applyAll(void (*func)(PCB*));

	PCB* first(); //nije implementirano
	PCB* last(); //nije implementirano

	PCB* find(PCB*); //ne verujem da treba
	PCB* findID(int);
	int isEmpty();
	int getSize();

	void clear();
	void add(PCB*);
	void push_back(PCB*);
	void push(PCB*);
	void remove(PCB*);
	void printList();
	int size;
	LinkedListNode* head, * tail;
};


/*Lista cuva elemente po kopiji, promeni da cuva po referenci*/

/*
	void insert_before(Iterator position, const T& e) {
		LinkedListNode* cur = position.elem;
		LinkedListNode* prev = cur->prev;

		LinkedListNode* _new = new LinkedListNode(e);

		_new->next = cur;
		_new->prev = prev;

		if (cur != NULL)
			cur->prev = _new;

		if (prev != NULL)
			prev->next = _new;

		if (head == cur)
			head = _new;

		size++;
	}

	void insert_after(Iterator position, const T& e) {
		LinkedListNode* cur = position.elem;
		LinkedListNode* next = cur->next;

		LinkedListNode* _new = new LinkedListNode(e);

		_new->prev = cur;
		_new->next = next;

		if (cur != NULL)
			cur->next = _new;

		if (next != NULL)
			next->prev = _new;

		if (tail == cur)
			tail = _new;

		size++;
	}
*/
#endif /* PCB_H_ */
