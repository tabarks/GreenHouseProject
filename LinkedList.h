#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED
struct LinkedList {
	int minutes;
	double sensorData;
	struct LinkedList *next;
};
void insertFirst(struct LinkedList **first, struct LinkedList *el);
int isMember(struct LinkedList **first, struct LinkedList *el);
void removeLinkedList(struct LinkedList **first, struct LinkedList *el);
void selectionSort(struct LinkedList **first);
struct LinkedList * newNode(int minutes, double sensorData);
void FreeLinkedList(struct LinkedList **first);

#endif