////////////////////////////////////////////////////////////////////////////////
//
// Cristinel Ababei
// cristinel.ababei@ndsu.edu
// DA LAND, NDSU
// CopyLeft
//
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <math.h>
#include "network.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
//
// GRAPH
//
////////////////////////////////////////////////////////////////////////////////

bool GRAPH::buid_graph_from_file( string filename)
{
	// construct the circuit graph;


	// (1) open file;
	ifstream network_ifstream;
	network_ifstream.open( filename.c_str());
	if ( !network_ifstream) {
		printf("\nError: Cannot open application graph file: %s\n",
			   filename.c_str());
		exit(1);
	} else {
		printf("\nOpened file <%s> for reading...\n", filename.c_str());
	}
	

	// (2) read lines from file; identify them by the first string;
    string line;
	while ( getline( network_ifstream, line) ) {

		istringstream iss( line);
		string tmp_string;
		iss >> tmp_string;
		printf("\n%s", tmp_string.c_str());


		// .model
		if ( tmp_string.compare(".model") == 0) {
			iss >> tmp_string;
			set_circuit_name( tmp_string);
			printf(" %s", _circuit_name.c_str());
		}


		// .inputs
		else if ( tmp_string.compare(".inputs") == 0) {
			while ( iss >> tmp_string) {
				NODE *new_input = add_input( tmp_string);
				printf(" %s", tmp_string.c_str());
				// this newly created input pad node is the driver/source
				// of a new net;
				NET *net = add_net( tmp_string, new_input->id(), INPUT_PAD);
				// this net whose driving node is this input should
				// be stored as fanout of the input pad node; note also
				// that input pad node do not have any fanins;
				new_input->add_fanout( net->id());
			}
		}


		// .outputs		
		else if ( tmp_string.compare(".outputs") == 0) {
			while ( iss >> tmp_string) {
				add_output( tmp_string);
				printf(" %s", tmp_string.c_str());
			}
		}


		// skip ".default*" lines;
		else if (tmp_string.compare(".default_input_arrival") == 0 ||
			tmp_string.compare(".default_output_required") == 0 ||
			tmp_string.compare(".default_input_drive") == 0 ||
			tmp_string.compare(".default_output_load") == 0) {
			continue;
		}


		// .gate
		else if ( tmp_string.compare(".gate") == 0) {
			iss >> tmp_string; // name such as "nand2", "xor", etc.;
			NODE *gate = add_gate( tmp_string);
			printf(" %s", tmp_string.c_str());

			// process inputs and output;
			while ( iss >> tmp_string) {
				// this gate input or output represents the name of a net,
				// which we add now, if it was not added already;
				// type is for sure LOGIC_GATE, because all inputs have been
				// already read in and nets with driver INPUT_PAD must
				// have been created by now;
				if ( tmp_string[0]=='O' && tmp_string[1]=='=') { // output;
					tmp_string.erase(0, 2);
					printf(" OUT: %s", tmp_string.c_str());	
					NET *net = add_net( tmp_string, gate->id(), LOGIC_GATE);
					gate->add_fanout( net->id());
					// set also the net_name which will be driven by this gate;
					gate->set_net_name( tmp_string);
				} else { // input a,b,c...;
					tmp_string.erase(0, 2);
					printf(" IN: %s", tmp_string.c_str());
					// Note: we use "-1" because it is possible that the fanin
					// gate has not been yet processed as a line that starts with 
					// ".gate"; we will do postprocessing later to correct the "-1"
					// with the actual gate id;
					NET *net = add_net( tmp_string, -1, LOGIC_GATE);
					gate->add_fanin( net->id()); 
					net->add_sink( gate->id());
				}				
			}
		}


		// .end
		else if ( tmp_string.compare(".end") == 0) {
			break;
		}
		else { // sanity check;
			printf("\nError: Input file is not in required format!\n");
			exit(1);
		}

	}//while

	network_ifstream.close();

	// (3) we still need to complete the connectivity between gates
	// who drive output pads and the corresponding output nodes;
	for ( int i = 0; i < _outputs_count; i ++) {
		int net_id = -1;
		for ( int j = 0; j < _nets_count; j ++) {
			if ( _outputs[i].name().compare( _nets[j].name()) == 0) {
				net_id = j;
				break;
			}
		}
		if ( net_id >= 0) {
			_outputs[ i].add_fanin( net_id);
			_nets[ net_id].add_sink( i);
		} else {
			printf("\nError: Floating output pad: %s \n",_outputs[i].name().c_str());
			exit(1);
		}
	}

	// (4) we still need to complete the connectivity for nets
	// whose source was set as "-1" earlier;
	for ( int j = 0; j < _nets_count; j ++) {
		if ( _nets[j].source() == -1) {
			int gate_id = -1;
			for ( int i = 0; i < _nodes_count; i ++) {
				if ( _nodes[i].net_name().compare( _nets[j].name()) == 0) {
					gate_id = i;
					break;
				}
			}
			if ( gate_id >= 0) {
				// gate_id represents the source id of this net that
				// had been recorded with "-1" above;
				_nets[j].set_source( gate_id);
			} else {
				printf("\nError: Net without driving gate: %s \n",_nets[j].name().c_str());
				exit(1);
			}
		}
	}

	
	printf("\n\nDone reading file <%s>.\n", filename.c_str());

	return true;
}

void GRAPH::print_graph( bool print_only_nets)
{
	// by default only nets are printed;

	printf("\nNetlist statistics:");
	if ( print_only_nets == false) {

	}
	printf("\nNETS %d", _nets_count);
	for ( int j = 0; j < _nets_count; j ++) {
		printf("\n%d %s <%s>  %s", _nets[j].id(),
			   (_nets[j].type() == INPUT_PAD ? "INPUT_PAD" : "LOGIC_GATE" ),
			   (_nets[j].type() == LOGIC_GATE ? _nodes[_nets[j].source()].name().c_str() : "-"),
			   _nets[j].name().c_str());
		printf("\n    Source: %d", _nets[j].source());
		printf("\n    Sinks:  ");
		for ( int k = 0; k < _nets[j].sinks_count(); k ++) {
			printf("%d ", _nets[j].sinks()[k]);
		}
	}

	printf("\n");
}
