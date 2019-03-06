// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// main.c


#include "specgraph.h"
#include "progexec.h"
#include "text_parsing.h"

// Remove blank spaces from string
void RemoveSpaces(char* str)
{
	// i and j serve as iterators
	char* i = str;
  	char* j = str;
  	while(*j != '\0')
  	{
    	*i = *j;
    	j++;
    	if(*i != ' ')
      		i++;
  	}
  	*i = '\0';
}


// The main function
int main(int argc, char *argv[]) {
	char * fileName = "makefile";
	Graph *g = createGraph();
	ArrayList *clist = readFile(fileName);
	int listsize = getSize(clist);
	for(int i = 0; i < listsize; i++) {
		// remove the blank spaces in target
		char * target = getTarget(getSpecList(clist)[i]);
		RemoveSpaces(target);
		//char ** depList = getDepList(getSpecList(clist)[i]);
		char *** cmdline = getCmdline(getSpecList(clist)[i]);
		Node * n = createNode(target);
		addToGraph(n, g); // Would exit if there are 2 same targets
		addCmdline(n, cmdline);
	}
	int gsize = getGraphSize(g);
	Node ** targets = getNodeList(g);
	for(int i = 0; i < gsize; i++) {
		char ** depList = getDepList(getSpecList(clist)[i]);
		int j = 0;
		while(depList[j] != NULL) {
			RemoveSpaces(depList[j]);
			Node * dep_n = createNode(depList[j]);
			// The new dependence node is not in graph
			if (checkExist(dep_n, getNodeList(g), getGraphSize(g)) == 0) {
				addToGraph(dep_n, g);
				if (addNodeSuccessor(targets[i], dep_n) == 1) {
					freeNode(dep_n);
				}
			// The new dependence node is already in graph
			} else {
				Node *dep_n1 = findNode(depList[j], g);
				addNodeSuccessor(targets[i], dep_n1);
				freeNode(dep_n);
			}
			j++;
		 }
	}
	if (checkCycle(g) == 0) {
		if(argc == 1) {
			//traversal(g);
			Node * node = getNodeList(g)[0];
			if (node == NULL) {
				fprintf(stderr, "%s\n", "Cannot find the specified target");
				exit(1);
			}
			traverseFromNode(g, node);
		} else if (argc == 2) {
			Node * node = findNode(argv[1], g);
			if (node == NULL) {
				fprintf(stderr, "%s\n", "Cannot find the specified target");
				exit(1);
			}
			traverseFromNode(g, node);
		} else {
			fprintf(stderr, "%s\n", "Too many arguments");
			exit(1);
		}
		
	} else {
		fprintf(stderr, "%s\n", "There is a cycle in the graph");
		exit(1);
	}	
	//printGraphInfo(g);
	freeGraph(g);
	freeArrayList(clist);
}

