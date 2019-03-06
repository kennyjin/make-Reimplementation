// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// progexec.h

#ifndef PROGEXEC
#define PROGEXEC

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>


void exeCmdline(char **cmdline);
int compare(char* target, char* dependence);

#endif