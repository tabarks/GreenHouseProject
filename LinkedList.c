#include "LinkedList.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdlib.h>
void insertFirst(struct LinkedList **first, struct LinkedList *el)
{
	el->next = *first;
	*first = el;
}
int isMember(struct LinkedList **first, struct LinkedList *el)
{
	struct LinkedList *temp = *first;
	while(temp!=NULL)
	{
		if(temp==el)
		return 1;
		else
		temp = temp->next;
	}
	return 0;
}
void removeLinkedList(struct LinkedList **first, struct LinkedList *el)
{
	struct LinkedList *temp = *first;
	if(temp==NULL)
	return;
	else if(temp->next==NULL)
	{
		if(temp==el)
		*first = NULL;
	}
	else
	{
		if(temp==el)
		{
			*first = temp->next;
			return;
		}
		while(temp->next!=NULL)
		{
			if(temp->next==el)
			{
				temp->next = temp->next->next;
				return;
			}
			temp = temp->next;
		}
	}
}

void selectionSort(struct LinkedList **first)
{
	struct LinkedList *temp, *it, *mx;
	temp = *first;
	*first = NULL;
	int i = 0;
	while(temp!=NULL)
	{
		mx = temp;
		it = temp->next;
		while(it!=NULL)
		{
			if(it->sensorData > mx->sensorData)
			mx = it;
			it = it->next;
		}
		removeLinkedList(&temp, mx);
		mx->next = NULL;
		insertFirst(first, mx);
		i++;
	}
}
struct LinkedList * newNode(int minutes, double sensorData)
{
	struct LinkedList *newPointer;
        //for(int i = 0; i < 10; i++)
        //{
          newPointer = (struct LinkedList*)malloc(1 * sizeof(struct LinkedList));
          //if(newPointer!=NULL)
            //break;
        //}
	if(newPointer==NULL){
	return NULL;
        }
	newPointer->minutes = minutes;
	newPointer->sensorData = sensorData;
	return newPointer;
};

void FreeLinkedList(struct LinkedList **first) ////////////////// Added
{
	
        struct LinkedList *temp, tm;
	temp = *first;
	while(temp!=NULL)
	{
		tm = *temp;
		free(temp);
		temp = tm.next;
	}
        
}