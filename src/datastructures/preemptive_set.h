#include <stdlib.h>
#include <string.h>
#include "arraylist.h"
#include "cellist.h"

#ifndef _PREEMPTIVE_SET_H_
#define _PREEMPTIVE_SET_H_


typedef struct{
    arraylist* values;
    cellist*   cells;
}preemptive_struct;

typedef struct {
	unsigned int size; 
	unsigned int capacity;
	preemptive_struct* body;
}prelist;


prelist* preemp_create();
void preemp_allocate(prelist* l, unsigned int size);
void preemp_add(prelist *p, preemptive_struct item);
void preemp_print(prelist* p);
void p_print(preemptive_struct ps);
preemptive_struct preemp_remove(prelist* l,unsigned int index);
preemptive_struct preemp_get(prelist* l, unsigned int index);


int same_row(preemptive_struct* p);
int same_col(preemptive_struct* p);

#endif