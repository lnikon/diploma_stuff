////////////////////////////////////////////////////////////////////////////////
//
// Cristinel Ababei
// cristinel.ababei@ndsu.edu
// DA LAND, NDSU
// CopyLeft
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "network.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
//
// main
//
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv)
{

	string network_filename = argv[1];


	struct timeval tv_start, tv_end;
	gettimeofday(&tv_start, (struct timezone*) NULL);


	// (1) build network from file;
	GRAPH circuit_network;
	circuit_network.buid_graph_from_file( network_filename);
	
	circuit_network.print_graph(); // debug;


	// (2) run simulator;
	//circuit_network.run_logic_simulator();
	//circuit_network.run_fault_simulator();


	// (3) report results;
	gettimeofday(&tv_end, (struct timezone*) NULL);		
	printf("\nRuntime: %ld sec \n", tv_end.tv_sec - tv_start.tv_sec);


	return 1;
}
