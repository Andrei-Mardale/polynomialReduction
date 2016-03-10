#include <stdio.h>
#include <stdlib.h>

#ifndef _GRAPH_
#define _GRAPH_

//initial and increase size for string
#define INCR 200
//select bit based on out destination node
#define BIT_SELECTOR(j) (1 << (j & 7)) 
//select matrix byte based on in and out node
#define EDGE(G, i, j) (G->m[i][(j / 8 + 1) & BIT_SELECTOR(j)])
//index value based on i, j
#define MAP_INDEX(G, i, j) ((i-1) * G->v + j)

//graph representation
typedef struct {
	int v; //size
	char **m; //bitwise adjacency matrix
} Graph;

typedef struct {
	int i, size; //index and size
	char *s; //actual value
} String;

//creates a new string data structure
String * newString();

//writes char * to string, and increases size if needed
int writeToString (String *S, const char *s);

void destroyString (String **S);

//read graph from file
Graph * read (FILE *in);

//perform reduction
String * hamiltonToSAT (Graph *G);

void destroyGraph (Graph **G);

#endif