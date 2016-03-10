#include "transform.h"

//functions used to check if graph has been read correctly
void printb(char c) {
	for (int i = 0; i < 8; ++i) {
		printf ("%d", (c & (1 << i)) != 0);
	}
}
void print (Graph *G) {
	int varsPerLine = G->v / 8 + 1;
	for (int i = 1; i <= G->v; ++i) {
		for (int j = 0; j < varsPerLine; ++j) {
			printb(G->m[i][j]);
		}
		printf("\n");
	}
}

int main (void) {
	//open input file
	FILE *in = fopen("test.in", "r");
	if (!in) {
		printf("Could not open file\n");
		return 1;
	}

	//read graph and close input file
	Graph *G = read(in);
	fclose(in);

	if(!G) {
		printf("Could not allocate memory\n");
		return 2;
	}

	//perform reduction
	String *S = hamiltonToSAT(G);

	destroyGraph(&G);

	if (!S) {
		printf("Could not allocate memory\n");
		return 2;
	}

	//open ouput file
	FILE *out = fopen("test.out", "w");
	if (!out) {
		printf("Could not open file\n");
		destroyString(&S);
	}

	//print boolean expression
	fprintf(out, S->s);

	destroyString(&S);
	fclose(out);
}