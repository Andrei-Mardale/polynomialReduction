#polynomialReduction
My fourth Algorithm Analysis homework

A C implementation of the polynomial reduction of the Hamiltonian Path Problem to the Boolean Satisfiability Problem.

The implemented algorithm is described here: http://www.csie.ntu.edu.tw/~lyuu/complexity/2011/20111018.pdf

#How to compile
"make build"

#How to run
"make run"

#Input
The input is an undirected graph given in the following format:
* V E (V - number of vertices, E - number of edges)
* u v - E such pairs which mean that there is a path from vertx u to vertex v

#Output
A boolean expression. If this expression is satisfiable, then there is a Hamiltonian Path in the given graph.
