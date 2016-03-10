#include "transform.h"

String * newString() {
	String *S = (String *) malloc(sizeof(String));
	if (!S) return NULL;
	
	S->s = (char *) calloc(INCR, sizeof(char));
	if (!S->s) {
		free(S);
		return NULL;
	}

	S->i = 0;
	S->size = INCR;

	return S;
}

//1 - allocation problem occured, 0 - ok
int writeToString (String *S, const char *s) {	//O(length(s))
	//do the writing
	while (*s != '\0') {
		S->s[S->i] = *s;
		++s;
		++S->i;
	}

	//check if size needs to be increased
	if (S->size - S->i < 50) {
		char *check;
		S->size += INCR;
		check = (char *) realloc(S->s, S->size * sizeof(char));
		if (!check) {
			free(S->s);
			free(S);
			return 1;
		}

		S->s = check;
	}

	return 0;
}

void destroyString (String **S) {
	free((*S)->s);
	free(*S);
	*S = NULL;
}

Graph * read (FILE *in) {
	//allocate graph
	Graph *G = (Graph *) calloc(1, sizeof(Graph));
	
	if (!G) return NULL;

	int edges;

	//read size
	fscanf(in, "%d %d", &G->v, &edges);

	//allocate adjacency matrix lines
	G->m = (char **) malloc((G->v + 1) * sizeof(char *));

	if (!G->m) {
		free(G);
		return NULL;
	}

	//how many chars per matrix line should be allocated
	int varsPerLine = G->v / 8 + 1;

	//allocate each line
	for (int i = 1; i <= G->v; ++i) {
		G->m[i] = (char *) calloc(varsPerLine, sizeof(char));
		if (!G->m[i]) {
			free(G->m);
			free(G);
			return NULL;
		}
	}

	//read each line
	int i, j;
	for (int k = 0; k < edges; ++k) {
		fscanf(in, "%d%d", &i, &j);
		EDGE(G, i, j) |= BIT_SELECTOR(j);
		EDGE(G, j, i) |= BIT_SELECTOR(i);
	}

	return G;
}

String * hamiltonToSAT (Graph *G) {
	String *S = newString();
	if (!S) return NULL;

	char *buffer = (char *) malloc(INCR * sizeof(char));

	if (!buffer) {
		free(S);
		return NULL;
	}

	//each node must appear at least once in the path
	for (int j = 1; j <= G->v; ++j) { //O(|V|^2)
		sprintf(buffer, "(x%d", MAP_INDEX(G, 1, j));
		writeToString(S, buffer);
		for (int i = 2; i <= G->v; ++i) { //O(|V|) 
			sprintf(buffer, "Vx%d", MAP_INDEX(G, i, j));
			writeToString(S, buffer);
		}
		sprintf(buffer,")^");
		writeToString(S, buffer);
	}

	//a node must not appear twice in the path
	for (int j = 1; j <= G->v; ++j) { //O(|V|^3)
		for (int i = 1; i <= G->v; ++i) { //O(|V|^2)
			int firstIndex = MAP_INDEX(G, i, j);
			//the two fors for k are in O(|V| - 1)
			for (int k = 1; k < i; ++k) {
				sprintf(buffer, "(~x%dV~x%d)^", firstIndex, MAP_INDEX(G, k, j));
				writeToString(S, buffer);
			}
			for (int k = i + 1; k <= G->v; ++k) {
				sprintf(buffer, "(~x%dV~x%d)^", firstIndex, MAP_INDEX(G, k, j));
				writeToString(S, buffer);
			}
		}
	}

	//no two nodes can take up the same position in the path
	for (int i = 1; i <= G->v; ++i) {//O(|V|^3)
		for (int j = 1; j <= G->v; ++j) {
			int firstIndex = MAP_INDEX(G, i, j);
			for (int k = 1; k < j; ++k) {
				sprintf(buffer, "(~x%dV~x%d)^", firstIndex, MAP_INDEX(G, i, k));
				writeToString(S, buffer);
			}
			for (int k = j + 1; k <= G->v; ++k) {
				sprintf(buffer, "(~x%dV~x%d)^", firstIndex, MAP_INDEX(G, i, k));
				writeToString(S, buffer);
			}
		}
	}

	//non-adjacent nodes cannot be adjacent in the path
	for (int i = 1; i <= G->v; ++i) { //O(|V|^2)
		for (int j = 1; j <= G->v; ++j) {
			if (!(EDGE(G, i, j) & BIT_SELECTOR(j))) {
				for (int k = 1; k < G->v; ++k) {
					sprintf(buffer, "(~x%dV~x%d)^", MAP_INDEX(G, k, i), MAP_INDEX(G, k + 1, j));
					writeToString(S, buffer);
				}
			}
		}
	}

	//clean output string
	if (S->s[S->i - 1] == '^') {
		S->s[S->i - 1] = '\0';
	}

	free(buffer);
	return S;
}

void destroyGraph (Graph **G) {
	for (int i = 1; i <= (*G)->v; ++i) {
		free((*G)->m[i]);
	}

	free((*G)->m);
	free(*G);
	*G = NULL;
}
