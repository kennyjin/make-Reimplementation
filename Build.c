// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// Build.c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define SIZE 1024
#define BUFFSIZE 1024

// This is actually single build specification
typedef struct array_ofstrings{
        char *target;
        char **dependences;
        char *** cmdline;
}Array_ofstrings;

// Create build spec
Array_ofstrings *createAS() {
        Array_ofstrings *construct = (Array_ofstrings*)calloc(1, sizeof(Array_ofstrings));
   	construct->dependences = (char**)calloc(BUFFSIZE, sizeof(char*));
        construct->cmdline = (char***)calloc(BUFFSIZE, sizeof(char**));
        for(int n = 0; n<BUFFSIZE; n++) {
		construct->cmdline[n] = (char**)calloc(BUFFSIZE, sizeof(char*));
        }
        return construct;
}

void freeConstruct(Array_ofstrings *construct) {
	if(construct != NULL) {
		if(construct -> target != NULL) {
			free(construct -> target);
			construct -> target = NULL;
		}
		if(construct -> dependences != NULL) {
			int i = 0;
			while(construct -> dependences [i] != NULL) {
				//printf("freed %s\n", construct -> dependences[i]);
				free(construct -> dependences[i]);
				construct -> dependences[i] = NULL;
				i++;
			}
			free(construct -> dependences);
			construct -> dependences = NULL;
		}
		if(construct -> cmdline != NULL) {
			for(int i = 0; i<BUFFSIZE; i++) {
				if(construct -> cmdline[i] != NULL) {
					int j = 0;
					while(construct -> cmdline[i][j] != NULL) {
						free(construct -> cmdline[i][j]);
						construct -> cmdline[i][j] = NULL;
						j++;
					}
					free(construct -> cmdline[i]);
					construct -> cmdline[i] = NULL;
				}
			}
			free(construct -> cmdline);
			construct -> cmdline = NULL;
		}
		free(construct);
		//construct = NULL;
	}
}

// This is an arraylist that stores build specs
typedef struct ArrayList {
	int size;
	int capacity;
	Array_ofstrings **buildSpec;
}ArrayList;

ArrayList *newlist() {
	Array_ofstrings **buildSpec = (Array_ofstrings**)calloc(SIZE, sizeof(Array_ofstrings*));
	ArrayList *list = calloc(1, sizeof(ArrayList));
	list->size = 0;
	list->capacity = SIZE;
	list->buildSpec = buildSpec;	
	return list;
}

void freeArrayList(ArrayList *list) {
	if(list != NULL) {
		if(list -> buildSpec != NULL) {
			for(int i = 0; i < (list -> size); i++) {
				if(list -> buildSpec[i] != NULL) {
					freeConstruct(list -> buildSpec[i]);
					list -> buildSpec[i] = NULL;
				}
			}
			free(list -> buildSpec);
			list -> buildSpec = NULL;
		}
		free(list);
		//list = NULL;
	}
	
}

// get size
int getSize(ArrayList *list) {
	return list -> size;
}

//get target
char* getTarget(Array_ofstrings *construct) {
	return construct -> target;
}

// get dependences
char** getDepList(Array_ofstrings *construct) {
	return construct -> dependences;
}


// get cmdline. Actually a 2d array of strings.
char *** getCmdline(Array_ofstrings *construct) {
	return construct -> cmdline;
}

// check if the list need to be expanded.
void check(ArrayList *list) {
	if(list->size == list->capacity) {
		printf("Expanding\n");
        	list->capacity = list->capacity * 2;
		list->buildSpec = realloc(list->buildSpec, list->capacity * sizeof(int));
		if (list->buildSpec == NULL) {
            		printf("realloc failed\n");
            		exit(1);
        	}
	}

}

// add spec to list
void add(ArrayList *list, Array_ofstrings* spec) {
    check(list);
    list->buildSpec[list->size] = spec;
    list->size++;
}

void printList(ArrayList *list) {
    printf("size of arraylist is: %d\n", list->size);
}

// void freePointer(ArrayList *list) {
	// free(construct->target);
	// for(int i = 0; i<BUFFSIZE; i++) {
	// 	free(construct->dependences[i]);
	// }
	// free(construct->dependences);
	// for(int j = 0; j<BUFFSIZE; j++) {
	// 	for(int k = 0; k<BUFFSIZE; k++) {
 //                	free(construct->cmdline[j][k]);
	// 	}
 //        }
	// free(construct);
	// for(int m = 0; m< BUFFSIZE; m++) {
	// 	free(list->buildSpec[m]);
	// }
	// free(list->buildSpec);
	// free(list);
	//printf("free them all\n");

// }

// Get the specification list
Array_ofstrings ** getSpecList(ArrayList *list){
	return list -> buildSpec;
}
