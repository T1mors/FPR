/** *************************************************************/
/* Functions and main function to calculate the Vertex-Cover	*/
/* for a given graph with the Highest-Degree Heuristic.		*/
/* In this heuristic the node with the highest degree is 	*/
/* covered in each iteration step, and all adjacent edges of 	*/
/* this node are removed. This is repeated until no edges are 	*/
/* left.							*/
/* 	T. Dewenter, January 2016				*/
/*	FPR-Master						*/
/*								*/
/* 	FRAGMENTED VERSION WHERE CODE NEEDS TO BE INSERTED	*/
/*								*/
/** *************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/** ********************* random_graph_matrix() *****************/
/* Function adds exactly m randomly chosen edges to the  	*/
/* adjacency matrix.						*/
/* (parts adapted from book "PG to Comp. Sim." by A.K. Hartmann)*/
/*                                           			*/
/* PARAMETERS: (*)= return-parameter                   		*/
/*	   N:		# of nodes in graph = # of variables	*/
/*	   M: 		# of constraints = # of edges		*/
/* (*) graph: 		adjacency matrix of graph      		*/
/*	seed:		seed for random no. gener.		*/
/*								*/
/* RETURNS:                                            		*/
/*     (nothing)                                       		*/
/****************************************************************/
void random_graph_matrix(int **graph, int N, int M)
{
  int added;
  int n1, n2;					/* ids of nodes, where an edge should be added */

  added = 0;
  while(added < M)             			/* insert edges with an uniform distribution */
  {
	n1 = (int) floor(drand48()*N);
	do
      	  n2 = (int) floor(drand48()*N);
    	while(n2==n1);             		/* choose pair of different nodes */

	if( graph[n1][n2]==0 ){			/* add edge if it does not exist so far */
	  graph[n1][n2] = 1;
	  graph[n2][n1] = 1;
	  added++;
 	}
  }
}


/** ****************** matrix_print() ***************************/
/* Prints matrix to stdout	                 		*/
/*								*/
/* PARAMETERS: (*)= return-parameter                    	*/
/*	   m: 		# of rows				*/
/*	   n:		# of columns				*/
/*    matrix:		matrix to print                         */
/*								*/
/* RETURNS:                                            		*/
/*     		(nothing)                       		*/
/****************************************************************/
void matrix_print(int m, int n, int **matrix)
{
  int i, j;

  for(i=0; i<m; i++){
    if(i > 0)
	printf("\n");
    for(j=0; j<n; j++)
	printf("%d ", matrix[i][j]);
  }
  printf("\n");
}

// Sum over all degrees in vector *deg
int sum_degree(int *deg, int N)
{
  int total_degree = 0;
  int i;
  for (i=0; i<N; i++){
	total_degree += deg[i];
  }

  return (total_degree);
}

/** ****************** high_degree_heur() ***********************/
/* Determines the vertex-cover of a graph by using a highest-	*/
/* degree heuristic which covers in each step the node with the */
/* highest degree.						*/
/*								*/
/* PARAMETERS: (*)= return-parameter                    	*/
/*         g:		adjacency matrix of graph               */
/*	   N:		# of nodes in graph			*/
/*								*/
/* RETURNS:                                            		*/
/*		number of covered nodes             		*/
/****************************************************************/
int high_degree_heur(int **g, int N)
{
  int i, j;
  int *deg, max_deg;
  int node_max_deg;
  int num_cov = 0;

  deg = (int *) calloc(N, sizeof(int));		// allocate memory and initialize with zeros
  
  for (i=0; i<N; i++){
	deg[i] = 0;
  }	

  // Compute degree for each node n in N
  for (i=0; i<N; i++){
	for (j=0; j<N; j++){
		deg[i] += g[i][j];
	}
  }
  
  // While one degree is greater than zero
  while (sum_degree(deg, N) > 0)
  {
	max_deg = deg[0];
	node_max_deg = 0;
	// Get vertex with highest degree and the index
	for (i=1; i<N; i++){
  		if (max_deg < deg[i]){
		  max_deg = deg[i];
		  node_max_deg = i;
		}
 	}
	
	num_cov += 1;
	deg[node_max_deg] = 0;
	
	// Decrease all adjacent vertices
	for (j=0; j<N; j++){
		if (g[node_max_deg][j]==1){
			g[node_max_deg][j]=0;
      // Set this to zero, otherwise the same edge will be taken two times into account
			g[j][node_max_deg]=0;
			deg[j] -= 1;
		}
		}
	}

  free(deg);
  return(num_cov);
}



/** ****************** main function: ***************************/
int main(int argc, char **argv)
{
  int i, j, argz=1;				// loop iterators
  int N, M, M_i, M_f;				// number of nodes, edges, and start and stop values
  int seed, run, num_runs;			// seed for random numer gener., loop iterator and number of random realisations
  int **adj_matrix;				// adjacency matrix; a_ij = 1, if i is neighbor of j; 0 else
  int step;					// iteration step of M
  int covered;					// number of covered nodes
  double sum_cov, sum_cov_sq;			// sum of number of covered vertices (and squared values)

  N = 6;
  M_i = 2;
  M_f = 15;
  seed = 1001;
  num_runs = 1000;


  if(argc==1){
    fprintf(stderr, "USAGE: %s {<arguments>}\n", argv[0]);
    fprintf(stderr, "     ARGUMENTS:\n");
    fprintf(stderr, "             <N>: # of vertices (d:%d)\n", N);
    fprintf(stderr, "           <M_i>: # of initial edges (d:%d)\n", M_i);
    fprintf(stderr, "           <M_f>: # of final edges (d:%d)\n", M_f);
    fprintf(stderr, "          <seed>: initial seed for random number generation (d:%d)\n", seed);
    fprintf(stderr, "      <num_runs>: number of runs with different seed (d:%d)\n", num_runs);
    fprintf(stderr, "   Example usage:\n");
    fprintf(stderr, "    %s %d %d %d %d %d\n", argv[0], N, M_i, M_f, seed, num_runs);
    exit(1);
  }


/* Read in parameters from the console: */
  N = atoi(argv[argz++]);
  M_i = atoi(argv[argz++]);
  M_f = atoi(argv[argz++]);
  seed = atoi(argv[argz++]);
  num_runs = atoi(argv[argz++]);

  step = (int) N/10;				// ^= steps of c=0.2
  if(step<=0)
    step = 1;


/* Do basic error checking: */
  if(M_i > M_f){
    fprintf(stderr, "Please choose final number of edges larger or equal to initial one. Exit!\n");
    exit(1);
  }

  if( M_f > (0.5*N*(N-1)) ){
    printf("There are too many edges to be inserted for c=%1.2f (%d > %d), i.e. the graph is \"overcomplete\".\n", 2.0*M_f/N, M_f, N*(N-1)/2);
    exit(1);
  }


/* Allocate memory: */
  adj_matrix = (int **) malloc(N*sizeof(int *));
  for(i=0; i<N; i++)
    adj_matrix[i] = (int *) malloc(N *sizeof(int));
  
  printf("# N = %d\n", N);
  printf("# M   c       cov     cov_err\n");


  srand48(seed);				/* set seed */

  M = M_i;
  while( M<=M_f ){						// loop over different number of edges
    sum_cov = 0.0;
    sum_cov_sq = 0.0;
    for(run=0; run<num_runs; run++){				// run over different seeds

	for(i=0; i<N; i++)					// Reset adjacency matrix
	  for(j=0; j<N; j++)
	    adj_matrix[i][j] = 0;


	random_graph_matrix(adj_matrix, N, M);			// Create random adjacency matrix

	covered = high_degree_heur(adj_matrix, N);		// Determine number of covered nodes with heuristic
	double a = (double) covered/N;
	sum_cov += a;
	sum_cov_sq += a*a;

    }

    printf("%d %f %f %e\n", M, 2.0*M/N, sum_cov/num_runs, sqrt(sum_cov_sq/num_runs - 
				(sum_cov/num_runs)*(sum_cov/num_runs))/sqrt(num_runs));		// Output: M, c, mean, standard deviation of mean
    M += step;
  }
  
  printf("\n");
  for(i=0; i<N; i++)
    free(adj_matrix[i]);
  free(adj_matrix);
  return(0);
}

