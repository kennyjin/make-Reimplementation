// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// progexec.c

#include "progexec.h"

// This function execute the cmdline
void exeCmdline(char **cmdline){
	pid_t  pid;
	int    status;
    int i = 0;
    while(cmdline[i] != NULL) {
        printf("%s ", cmdline[i]);
        i++;
    }
    printf("\n"); 
    if ((pid = fork()) < 0) {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
         if (execvp(*cmdline, cmdline) < 0) {     /* execute the command  */
         	printf("*** ERROR: exec failed\n");
          	exit(1);
         }
    }
    else {                                  /* for the parent:      */
    	while (wait(&status) != pid)       /* wait for completion  */
         ;
    }
}

// Return 1 if dependence file is "newer" than target. Thus we need to update target.
// Return 0 if dependence file is modified before target. Than no update is needed.
int compare(char* target, char* dependence) {

    struct stat statbuf;
    if(stat(target, &statbuf) != 0) {
        fprintf(stderr, "Failed to get the stat of %s\n", target);
        exit(-1);
    }
    long int t1 = statbuf.st_mtime; // The modification time of target
    if(stat(dependence, &statbuf) != 0) {
        fprintf(stderr, "Failed to get the stat of %s\n", dependence);
        exit(-1);
    }
    long int t2 = statbuf.st_mtime; // The modification time of dependence
    if(t2 > t1) {
        return 1; // The target file need to be updated
    }
    return 0;
}

