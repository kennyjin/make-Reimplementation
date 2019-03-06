// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// text_parsing.c

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Build.h"
#define BUFFSIZE 1024
#define SIZE 1024

void freeString(char **str) {
	if(*str != NULL) {
		free(*str);
		*str = NULL;
	}
}

// Read file and return an arraylist of build spec
ArrayList *readFile(char* argv) {
	char c;
	char ch = ':';
	int p = 0;
	char *filename = argv;
	FILE *fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL)
    	{
        	printf("Cannot open makefile \n");
        	fptr = fopen("Makefile", "r");
        	if(fptr == NULL) {
        		printf("open Makefile \n");
        		exit(1);
        	}
    	}

	int num_cmd = 0;
	int line_counter = 0;
	ArrayList *list = newlist();
	Array_ofstrings *construct = NULL;
	
	while(1){
		char *str = calloc(BUFFSIZE , sizeof(char));
		//char *str_pointer = calloc(BUFFSIZE , sizeof(char));
		char *str_pointer = NULL;
		char *str_cmd = calloc(BUFFSIZE , sizeof(char));
		char *str_whole = calloc(BUFFSIZE , sizeof(char));
		int j = 0; // Num of characters in the dependence buffer, aka str_pointer
		int m = 0; // Num of dependences in the struct
		int q = 0; // Num of characters in the cmdline buffer, aka str_cmd
		int flag = 1; // 0 indicates there is a target in the line
		int cmd = 1;
		int first_char = 0;
		int check_line = 0;
		int comment_line = 0;
	
		for(int i = 0; i<BUFFSIZE; i++){	
			c = fgetc(fptr);
			str_whole[i] = c;
			if(c == EOF) { 
				//no newline before EOF
				if(q!= 0) {
					construct->cmdline[num_cmd][p] = str_cmd;
					line_counter++;
					freeString(&str);
					//freeString(&str_pointer);
					////freeString(&str_whole);
				} else {
					freeString(&str_cmd);
				}
				if(j != 0) {
					construct->dependences[m] = str_pointer; 
					line_counter++;
					//freeString(&str); //danger!!
					// str = NULL;
					//freeString(&str_cmd);
					//str_cmd = NULL;
					////freeString(&str_whole);
					//str_whole = NULL;
				}
				if(flag == 1) {
					freeString(&str);
				}
				//freeString(&str); // danger!!!
				//str = NULL;
				////freeString(&str_cmd);
				// str_cmd = NULL;
				////freeString(&str_pointer);
				// str_pointer = NULL;
				freeString(&str_whole);
				//str_whole = NULL;
				return list;
			}
			if(c != '\n') {
				check_line++;
				if(first_char == 0 && c == '#') {
				//when the first character in the line is #
					first_char = 1;
					comment_line = 1;
					freeString(&str);
					//str = NULL;
					//freeString(&str_cmd);
					//str_cmd = NULL;
					//freeString(&str_pointer);
					//str_pointer = NULL;
					continue;
				}
		       	if(first_char == 1) {
					continue;
		       	}
		       	if(line_counter == 0 && c == '\t') {
		       		first_char = 1;
		       		continue;
		       	}

				if(c == ch) {
					str[i] = '\0';
					construct = createAS();
					num_cmd = 0;
					flag = 0;
					str_pointer = calloc(BUFFSIZE, sizeof(char));
					//printf("%s\n", "alloc'd for dependence");
					construct->target = str;
					//str = calloc(BUFFSIZE , sizeof(char));
					//add to arraylist according to target number	
					add(list, construct);
					freeString(&str_cmd);
					//str_cmd = NULL;
					continue;

				}
				if(flag) {
					str[i] = c;
				}
				if(flag == 0) {
					if(c!= ' '){
						str_pointer[j] = c;
						j++;
					}
					if(c == ' ' && j!=0) {
						//clear str_pointer one character by one character
						construct->dependences[m] = str_pointer; 
						//printf("%s\n", str_pointer);
						str_pointer = calloc(BUFFSIZE , sizeof(char));
						//printf("%s\n", "alloc'd for dependence");
						j = 0;
						m++;
						//freeString(&str_cmd);
						//str_cmd = NULL;  
					}

				}
				if(c == '\t'){
					if(q != 0){
						//when cmdline is separated by tab
						construct->cmdline[num_cmd][p] = str_cmd;
                        str_cmd = calloc(BUFFSIZE , sizeof(char));
                        q = 0;
                        p++;
                        //freeString(&str);
						//str = NULL;
						//freeString(&str_pointer);
						//str_pointer = NULL;   
						continue;
					}
					//else when it's the beginning of one line
					p = 0;
					cmd = 0;
					continue;
				}
				if(cmd == 0) {//when the line starts with the tab
                    if(c != ' ') {
                       	str_cmd[q] = c;
                        q++;
					}
					if(c == ' ' && q != 0){ //when cmdline is separated by several space
						if(strcmp(str_cmd, "") != 0) {
							construct->cmdline[num_cmd][p] = str_cmd;
							str_cmd = calloc(BUFFSIZE , sizeof(char));
						}
						
                      	//freeString(&str); // Danger, cause seg fault!
						// str = NULL;
						//freeString(&str_pointer);
						// str_pointer = NULL;        
						q = 0;
						p++;
			
					}
			
                }

				if(i == BUFFSIZE -1){ 
					//when line is too long
					fprintf(stderr, "%d: Invalid line "" %s""\n", line_counter, str_whole);
					for(int a = 0; a<i; a++) {
						str_whole[a] = 0;
					}
					exit(1);
				}
					
			}
			if(c == '\n') {
				if(flag == 1) {
					freeString(&str);
				}
				if(check_line > 0) {
					if(line_counter == 0) {
						//check the first line if it is target line
							if(flag) {
								fprintf(stderr, "%s is the first line but isn't the target line\n", str_whole);
								exit(1);
							}
					}
					if(cmd && flag) {
						//ignore the comment line
						if(comment_line) {
							break;
						}
						printf("%d: Invalid line "" %s""\n", line_counter, str_whole);
						for(int b = 0; b<i; b++) {
                            str_whole[b] = 0;
                        }
						exit(1);
					}
					if(j!= 0) {
						construct->dependences[m] = str_pointer; 
						//printf("%s\n", str_pointer);
						//freeString(&str_cmd);
						//str_cmd = NULL;
						//str_pointer = calloc(BUFFSIZE , sizeof(char));
					} else { // When there are actually no dependence
						freeString(&str_pointer);
						//str_pointer = NULL;
					}
					
					if(!cmd || !flag) {
						//when it is cmdline
						if(cmd == 0) {
                        			//since 6 commands only have 5 spaces, add the last name into construct_cmdline
                            if(p!= 0) {
							//in case the cmdline is really long
                                construct->cmdline[num_cmd][p] = str_cmd;
                                freeString(&str);
								//str = NULL;
								//freeString(&str_pointer);
								//str_pointer = NULL;
								//str_cmd = calloc(BUFFSIZE , sizeof(char));
	                            num_cmd++;
							} else {
								freeString(&str_cmd);
								//str_cmd = NULL;
							}
							if(num_cmd >= BUFFSIZE){
								fprintf(stderr, "%d: Invalid line: %s\n", line_counter, str_whole);
                                exit(1);
							}

                    	}
						//when it is target and dependences line
	                    line_counter++;
                	    break;
					}

				} else {
					//when it is the empty line
					freeString(&str);
					//str = NULL;
					freeString(&str_cmd);
					//str_cmd = NULL;
					//freeString(&str_pointer);
					//str_pointer = NULL;
                	break;
				}
			}

		}
		// if(*str == '0') {
		// 	////freeString(str);
		// 	str = NULL;
		// }
		// if(str_cmd != NULL && strcmp(str_cmd,"")) {
		// 	////freeString(str_cmd);
		// 	str_cmd = NULL;
		// }
		// if(str_pointer != NULL && strcmp(str_pointer,"")) {
		// 	////freeString(str_pointer);
		// 	str_pointer = NULL;
		// }
		free(str_whole);
		str_whole = NULL;

	}

}



