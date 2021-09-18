#include "PCB.h"
#define NULL 0
#include <STDIO.h>
PCBList::PCBList() : size(0), head(NULL), tail(NULL) {}

PCBList::~PCBList(){
	clear();
}

void PCBList::applyAll(void (*func)(PCB*)){
	printf("..%d\n", head);
	for(LinkedListNode* cur = head; cur != NULL; cur = cur->next){
		printf("jhg\n");
		func(cur->data);
	}
}

/*void PCBList::unblockAll(){
	for(LinkedListNode* cur = head; cur && cur->next; cur = cur->next){
		(cur->data)->unblock();
	}
}*/

PCB* PCBList::findID(int _idd){
	for(LinkedListNode* cur = head; cur != NULL; cur = cur->next){
		if((cur->data)->getID() == _idd)
			return cur->data;
	}
	return 0;
}

int PCBList::isEmpty(){
	return size==0;
}

int PCBList::getSize(){
	return size;
}

void PCBList::clear(){
	LinkedListNode* e = head;
	while (e != NULL) {
		head = head->next;
		e->next = e->prev = NULL;
		delete e;
		e = head;
		size--;
	}
	head = tail = NULL;
}

void PCBList::add(PCB* pcb){
	LinkedListNode* newE = new LinkedListNode(pcb);
		if (isEmpty()) {
			head = newE;
		}
		else {
			tail->next = newE;
			newE->prev = tail;
		}
		tail = newE;
		size++;
}

void PCBList::push_back(PCB* pcb){
	add(pcb);
}

void PCBList::push(PCB* pcb){
	LinkedListNode* newE = new LinkedListNode(pcb);
	if(isEmpty()){
		tail = newE;
	}
	else{
		head->prev = newE;
		newE->next = head;
	}
	head = newE;
	size++;
}

void PCBList::remove(PCB* pcb){
	LinkedListNode* cur;
	for(cur = head; cur ; cur = cur->next){
		if(cur->data == pcb)
			break;
	}
	if (cur == 0) return;
	if (cur == head) {
		head = head->next;
	}
	if (cur == tail) {
		tail = tail->prev;
	}

	LinkedListNode* next = cur->next, * prev = cur->prev;

	if (prev != NULL) {
		prev->next = next;
	}
	if (next != NULL) {
		next->prev = prev;
	}

	cur->next = cur->prev = NULL;
	size--;

	delete cur;
}

void PCBList::printList(){
	printf("PCBList: ");
	for(LinkedListNode* cur = head; cur != NULL; cur = cur->next){
		printf("%d ", (cur->data)->getID());
	}
	printf("\n");
}

PCBList::LinkedListNode::LinkedListNode(PCB *d) : data(d), next(NULL), prev(NULL) {}

