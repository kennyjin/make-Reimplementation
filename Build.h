// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// Build.c

#ifndef HEADER_BUILD
#define HEADER_BUILD

typedef struct array_ofstrings {
	char* target;
	char** dependences;
	char*** cmdline;
}Array_ofstrings;

Array_ofstrings *createAS();

typedef struct ArrayList{
	int size;
  	int capacity;
	Array_ofstrings** buildSpec;	

}ArrayList;

ArrayList *newlist();

void check(ArrayList *list);

void add(ArrayList *list, Array_ofstrings* spec);

void printList(ArrayList *list); 

void freeArrayList(ArrayList *list);

void freeConstruct(Array_ofstrings *construct);

int getSize(ArrayList *list);

char* getTarget(Array_ofstrings *construct);

char** getDepList(Array_ofstrings *construct);

char *** getCmdline(Array_ofstrings *construct);

Array_ofstrings ** getSpecList(ArrayList *list);


#endif
