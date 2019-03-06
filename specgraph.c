// Kenny Jin jjin59@wisc.edu
// Ying Zhang yzhang865@wisc.edu
// specgraph.c
#include "specgraph.h"

// Create a node for the graph using data
Node *createNode(char* data) {
	Node *node;
	if ((node = malloc(sizeof(Node))) == NULL) {
		fprintf(stderr, "%s\n", "Failed to create node");
		exit(1);
	}
	node -> data = data;
	if ((node -> successors = malloc(LISTSIZE * sizeof(Node))) == NULL) {
		fprintf(stderr, "%s\n", "Failed to create node");
		exit(1);
	}
	node -> num_succ = 0;
	node -> cmdline = NULL;
	node -> num_cmd = 0;
	return node;
}

void freeNode(Node *node) {
	if(node != NULL) {
		if(node -> successors != NULL) {
			free(node -> successors);
			node -> successors = NULL;
		}
		// if(node -> data != NULL) {
		// 	free(node -> data);
		// }
		free(node);
		//node = NULL;
	}
	
}

// This function adds the cmdline to the node
void addCmdline(Node *node, char*** cmdline){
	node -> cmdline = cmdline;
}

// Initialize the graph
Graph *createGraph() {
	Graph *graph;
	if ((graph = malloc(sizeof(Graph))) == NULL) {
		fprintf(stderr, "%s\n", "Failed to create graph");
		exit(1);
	}
	graph -> num_node = 0;
	graph -> capacity = LISTSIZE;
	if ((graph -> nodeList = malloc(graph -> capacity * sizeof(Node))) == NULL) {
		fprintf(stderr, "%s\n", "Failed to create graph");
		exit(1);
	}
	return graph;
}

void freeGraph(Graph * g) {
	if(g != NULL) {
		if(g -> nodeList != NULL) {
			for(int i = 0; i < g -> num_node; i++) {
				freeNode(g -> nodeList[i]);
				g -> nodeList[i] = NULL;
			}
			free(g -> nodeList);
			g -> nodeList = NULL;
		}
		free(g);
		//g = NULL;
	}
	

}


int getGraphSize(Graph *g) {
	return g -> num_node;
}

// Get the number of successors
int getNumSucc(Node *node) {
	return node -> num_succ;
}

// This function add a node to a graph
void addToGraph(Node *node, Graph *graph) {
	//TODO need to use realloc
	if (graph -> num_node == graph -> capacity) {
		graph->capacity = graph->capacity * 2;
		graph -> nodeList = realloc(graph -> nodeList, graph->capacity * sizeof(Node));
	}
	if(checkExist(node, getNodeList(graph), graph -> num_node) == 1){
		fprintf(stderr, "Node \"%s\" already in the graph\n", getData(node));
		exit(1);
	}
	graph -> nodeList[graph -> num_node] = node;
	graph -> num_node ++;
}

// This function add "succ" to the successor list of node "node".
// This is equivalent to add an edge from "node" to "succ", since this is a directed graph 
// return 1 on failure, 0 on success
int addNodeSuccessor(Node *node, Node *succ) {
	if (node -> num_succ == LISTSIZE) {
		fprintf(stderr, "%s\n", "Too many successors");
		exit(1);
	}
	if(checkExist(succ, getSuccessorList(node), node -> num_succ) == 1){
		fprintf(stderr, "Node \"%s\" already in the successor list of ", getData(succ));
		fprintf(stderr, "\"%s\"\n", getData(node));
		return 1;
	}
	node -> successors [node -> num_succ] = succ;
	node -> num_succ ++;
	return 0;
}

char* getData(Node *node) {
	return node -> data;
}

Node ** getSuccessorList(Node *node) {
	return node -> successors;
}

Node ** getNodeList(Graph *graph) {
	return graph -> nodeList;
}

// Find a node in the graph using a string "data"
Node * findNode(const char* data, Graph *g) {

	for(int i = 0; i < g -> num_node; i++) {
		char *a = getData(g -> nodeList[i]);
		if (strcmp(a, data) == 0) {
			return g -> nodeList[i];
		}
	}
	return NULL;
}

// Print the info about successor list
void printSucc(Node *node) {
	printf("Node %s ", getData(node));
	printf("successors: ");
	Node **succ_list = getSuccessorList(node);
	for (int i = 0; i < (node -> num_succ); i++) {
		printf("%s ",getData(succ_list[i]));
	}
	printf("\n");
}

// Print the info about graph
void printGraphInfo(Graph *graph) {
	printf("%d nodes are in the graph.\n", graph -> num_node);
	for(int i = 0; i < (graph -> num_node); i++) {
		printSucc(graph -> nodeList [i]);
		printCmdline(graph -> nodeList [i]);
	}
}

// This function checks if the node already exist in the node list.
// It regards nodes with the same data as the same node
// no matter what the successor list of the node is.
// returns 0 if there is no same node in the list, 1 otherwise
int checkExist(Node *node, Node **list, int listsize) {
	for(int i = 0; i < listsize; i++) {
		char *a = getData(list[i]);
		char *b = getData(node);
		if (strcmp(a, b) == 0) {
			return 1;
		}
	}
	return 0;
}

// This function checks if the node is in the graph
int checkNodeGraph(Node *node, Graph *graph) {
	return checkExist(node, graph -> nodeList, graph -> num_node);
}

// This function checks if the graph contains a cycle
// return 0 for no cycle, 1 otherwise
int checkCycle(Graph *graph) {
	Node *visited[graph -> capacity];
	int count_visited = 0;
	Node *stack[graph -> capacity];
	int count_stack = 0;
	Node **list = getNodeList(graph);
	for(int i = 0; i < (graph -> num_node); i++) {
		if(checkExist(list[i], visited, count_visited)){
			continue;
		}
		Node *curr = list[i];
		// Put curr into visited list
		visited[count_visited] = curr;
		count_visited++;
		stack[count_stack] = curr;
		count_stack++;
		while(count_stack > 0) {
			curr = stack[count_stack - 1];
			// If the current node has no successors, pop it from the stack
			if (curr -> num_succ == 0) {
				count_stack--;
				continue;
			}
			// Check every successor, determine if it is visited and in the stack
			// If the successor is both visited and in the stack, there is a cycle, return 1
			Node ** temp = getSuccessorList(curr);
			// This is supposed to be the index of the "next" node to be pushed to the stack
			int index = curr -> num_succ; 
			for(int j = 0; j < (curr -> num_succ); j++) {
				int in_v = checkExist(temp[j], visited, count_visited);
				int in_s = checkExist(temp[j], stack, count_stack);
				// There is indeed a cycle, return 1
				if (in_v == 1 && in_s == 1){
					return 1;
				}
				if (in_v == 0 && in_s == 0 && j < index){
					index = j;
				} 
			}
			if (index == curr -> num_succ) {
				count_stack--;
				continue;
			}
			stack[count_stack] = temp[index];
			count_stack++;
			visited[count_visited] = temp[index];
			count_visited++;
		}
	}
	return 0;
}

// This function traverse the graph using post order traversal
// execute command lines as well
void traversal(Graph *graph) {
	Node *visited[graph -> capacity];
	int count_visited = 0;
	Node *stack[graph -> capacity];
	int count_stack = 0;
	Node **list = getNodeList(graph);
	for(int i = 0; i < (graph -> num_node); i++) {
		if(checkExist(list[i], visited, count_visited)){
			continue;
		}
		Node *curr = list[i];
		// Put curr into visited list
		visited[count_visited] = curr;
		count_visited++;
		stack[count_stack] = curr;
		count_stack++;
		while(count_stack > 0) {
			curr = stack[count_stack - 1];
			// If the current node has no successors, pop it from the stack
			if (curr -> num_succ == 0) {
				count_stack--;
				continue;
			}
			// Check every successor, determine if it is visited and in the stack
			// If the successor is both visited and in the stack, there is a cycle, return 1
			Node ** temp = getSuccessorList(curr);
			// This is supposed to be the index of the "next" node to be pushed to the stack
			int index = curr -> num_succ; 
			for(int j = 0; j < (curr -> num_succ); j++) {
				int in_v = checkExist(temp[j], visited, count_visited);
				int in_s = checkExist(temp[j], stack, count_stack);
				// There is indeed a cycle, return 1
				if (in_v == 1 && in_s == 1){
					printf("cycle exists\n");
					return;
				}
				if (in_v == 0 && in_s == 0 && j < index){
					index = j;
				} 
			}
			// There is no unvisited successors for the current node
			if (index == curr -> num_succ) {
				for(int j = 0; j < (curr -> num_succ); j++) {
					// When the target file needs to be updated, execute cmd
					if (access(getData(curr), F_OK ) == -1||compare(getData(curr), getData(temp[j])) == 1) {
						char ***cmdline = curr -> cmdline;
						int k = 0;
						if(access(getData(curr), F_OK ) != -1) {
							printf("%s exists\n", getData(curr));
						}
						while(cmdline[k] != NULL && cmdline[k][0] != NULL) {
							exeCmdline(cmdline[k]);
							k++;
						}
						break;
					}
				}
				count_stack--;
				continue;
			}
			stack[count_stack] = temp[index];
			count_stack++;
			visited[count_visited] = temp[index];
			count_visited++;
		}
	}
}

// This function traverse the graph using post order traversal
// execute command lines as well
// Specify a starting node of traversal using node
void traverseFromNode(Graph *graph, Node *node) {
	Node *visited[graph -> capacity];
	int count_visited = 0;
	Node *stack[graph -> capacity];
	int count_stack = 0;
	Node *curr = node;
	// Put curr into visited list
	visited[count_visited] = curr;
	count_visited++;
	stack[count_stack] = curr;
	count_stack++;
	while(count_stack > 0) {
			curr = stack[count_stack - 1];
			// If the current node has no successors, pop it from the stack
			if (curr -> num_succ == 0) {
				char ***cmdline = curr -> cmdline;
				int k = 0;
				while(cmdline != NULL && cmdline[k] != NULL && cmdline[k][0] != NULL) {
					exeCmdline(cmdline[k]);
					k++;
				}
				count_stack--;
				continue;
			}
			// Check every successor, determine if it is visited and in the stack
			// If the successor is both visited and in the stack, there is a cycle, return 1
			Node ** temp = getSuccessorList(curr);
			// This is supposed to be the index of the "next" node to be pushed to the stack
			int index = curr -> num_succ; 
			for(int j = 0; j < (curr -> num_succ); j++) {
				int in_v = checkExist(temp[j], visited, count_visited);
				int in_s = checkExist(temp[j], stack, count_stack);
				// There is indeed a cycle, return 1
				if (in_v == 1 && in_s == 1){
					printf("cycle exists\n");
					return;
				}
				if (in_v == 0 && in_s == 0 && j < index){
					index = j;
				} 
			}
			// There is no unvisited successors for the current node
			if (index == curr -> num_succ) {
				// When there is no successors, just execute command
				for(int j = 0; j < (curr -> num_succ); j++) {
					// When the target file needs to be updated, execute cmd
					if (access(getData(curr), F_OK ) == -1||compare(getData(curr), getData(temp[j])) == 1) {
						char ***cmdline = curr -> cmdline;
						int k = 0;
						while(cmdline[k] != NULL && cmdline[k][0] != NULL) {
							exeCmdline(cmdline[k]);
							k++;
						}
						break;
					}

				}
				count_stack--;
				continue;
			}
			stack[count_stack] = temp[index];
			count_stack++;
			visited[count_visited] = temp[index];
			count_visited++;
		}
}

// Print the cmdlines of node
void printCmdline(Node* node){
	char*** cmdline = node -> cmdline;
	if(cmdline == NULL) {
		return;
	}
	int i = 0;
	while(cmdline[i] != NULL) {
		int j = 0;
		while(cmdline[i][j] != NULL) {
			printf("%s ", cmdline[i][j]);
			j++;
		}
		i++;
		if (j == 0) {
			continue; // There is actually nothing, so do not go to a new line
		}
		printf("\n");
	}
	printf("\n");
}