#ifndef SPECGRAPH
#define SPECGRAPH

#include "progexec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define LISTSIZE 1024
#define BUFFSIZE 1024

typedef struct node Node;
typedef struct graph Graph;
Node *createNode(char* data);
int addNodeSuccessor(Node *node, Node *succ);
char* getData(Node *node);
void printSucc(Node *node);
Node ** getSuccessorList(Node *node);
Graph *createGraph();
void addToGraph(Node *node, Graph *graph);
void printGraphInfo(Graph *graph);
Node ** getNodeList(Graph *graph);
int checkExist(Node *node, Node **list, int listsize);
void addCmdline(Node *node, char*** cmdline);
int checkNodeGraph(Node *node, Graph *graph);
void printCmdline(Node* node);
void traversal(Graph *graph);
int getGraphSize(Graph *g);
// Get the number of successors
int getNumSucc(Node *node);
int checkCycle(Graph *graph);
Node * findNode(const char* data, Graph *g);
void traverseFromNode(Graph *graph, Node *node);
void freeGraph(Graph * g);
void freeNode(Node *node);

struct node {
	char* data; // The data the node stores
	Node ** successors;
	int num_succ; // The number of successors
	char *** cmdline;
	int num_cmd; // The number of command lines
};

struct graph {
	int num_node; // The number of nodes
	Node ** nodeList;
	int capacity;
};

#endif