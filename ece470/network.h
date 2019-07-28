////////////////////////////////////////////////////////////////////////////////
//
// Cristinel Ababei
// cristinel.ababei@ndsu.edu
// DA LAND, NDSU
// CopyLeft
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _NETWORK_H_
#define _NETWORK_H_
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

enum NODE_TYPE { INPUT_PAD=0, OUTPUT_PAD=1, LOGIC_GATE=2, UNKNOWN=3 };


////////////////////////////////////////////////////////////////////////////////
//
// NODE
//
////////////////////////////////////////////////////////////////////////////////

class NODE {
	// represents a logic gate or an IO pad;
 private:
	int _id;
	string _name; // name of IO or name of gate;
	string _net_name; // name of net which this node drives;
	NODE_TYPE _type;
	int _fanin_count;
	int _fanout_count;	
	vector<int> _fanin; // ids of input nets; 
	vector<int> _fanout; // id of the fanout net whose source this node is;
	double _delay;

 public:
	NODE() { 
		_id = -1;
		_type = UNKNOWN;
		_fanin_count = 0; 
		_fanout_count = 0;
		_delay = 1.0;
	}
	NODE( int id, string name, NODE_TYPE type) :
		_id(id), _name(name), _type(type) {
		_fanin_count = 0; 
		_fanout_count = 0;
		_delay = 1.0;
	}
	~NODE() {}
	
	int id() const { return _id; }
	string name() const { return _name; }
	string net_name() const { return _net_name; }
	void set_net_name( string net_name) { _net_name = net_name; }
	NODE_TYPE type() { return _type; }
	double delay() const { return _delay; }
	vector<int> &fanin() { return _fanin; }
	vector<int> &fanout() { return _fanout; }

	void add_fanin( int net_id) {
		_fanin.push_back( net_id);
		_fanin_count ++;
	}
	void add_fanout( int net_id) { // id is index of node this one fanouts to;
		_fanout.push_back( net_id);
		_fanout_count ++;
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// NET
//
////////////////////////////////////////////////////////////////////////////////

class NET {
	// a net in the circuit; can have its own delay;
 private:
	int _id;
	string _name; // name of net;
	int _source; // id of node that is source of this net;
	NODE_TYPE _type; // type IN or GATE of the driver/source of this net;
	int _sinks_count;
	vector<int> _sinks; // id of destination nodes; basically its fanout;
	double _delay;
		
 public:
	NET( int id, string name, int source, NODE_TYPE type_of_driver) :
		_id(id), _name(name), _source(source), _type(type_of_driver) {
		_sinks_count = 0;
		_delay = 1.0;
	}
	~NET() {}
	
	int id() const { return _id; }
	string name() const { return _name; }
	int source() { return _source; }
	void set_source( int source) { _source = source; }
	NODE_TYPE type() { return _type; }
	int sinks_count() { return _sinks_count; }
	vector<int> &sinks() { return _sinks; }
	void add_sink( int id) {
		_sinks.push_back( id);
		_sinks_count ++;
	}
	double delay() const { return _delay; }
	void set_delay( double delay) { _delay = delay; }
};

////////////////////////////////////////////////////////////////////////////////
//
// GRAPH
//
////////////////////////////////////////////////////////////////////////////////

class GRAPH {
	// basically, the circuit netlist;
 private:
	int _inputs_count;
	int _outputs_count;
	int _nodes_count;
	int _nets_count;
	vector<NODE> _inputs; // input pads;
	vector<NODE> _outputs; // output pads;
	vector<NODE> _nodes; // logic gates;
	vector<NET> _nets; // interconnects;
	string _circuit_name;

 public:
	GRAPH() {
		_inputs_count=0; _outputs_count=0;
		_nodes_count=0; _nets_count=0;
	}
	~GRAPH() {}
	
	int inputs_count() { return _inputs_count; }
	int outputs_count() { return _outputs_count; }
	int nodes_count() { return _nodes_count; }
	int nets_count() { return _nets_count; }
	void set_circuit_name( string circuit_name)
		{ _circuit_name = circuit_name; }
	vector<NODE> &inputs() { return _inputs; }
	vector<NODE> &outputs() { return _outputs; }
	vector<NODE> &nodes() { return _nodes; }
	vector<NET> &nets() { return _nets; }

	NODE *add_input( string name) {
		_inputs.push_back( NODE(_inputs_count, name, INPUT_PAD));
		_inputs_count ++;
		return &_inputs[ _inputs_count - 1]; // return the input just created;
	}
	NODE *add_output( string name) {
		_outputs.push_back( NODE(_outputs_count, name, OUTPUT_PAD));
		_outputs_count ++;
		return &_outputs[ _outputs_count - 1];
	}
	NODE *add_gate( string name) {
		_nodes.push_back( NODE(_nodes_count, name, LOGIC_GATE));
		_nodes_count ++;
		return &_nodes[ _nodes_count - 1]; // return the gate just created;
	}
	NET *add_net( string name, int src_id, NODE_TYPE type) {
		// return the id of the net created already or we create 
		// it now, once only;
		int net_id = -1;
		for ( int j = 0; j < _nets_count; j ++) {
			if ( name.compare( _nets[j].name()) == 0) {
				net_id = j;
				break;
			}
		}
		if ( net_id >= 0) {
			return &_nets[ net_id];
		} else {
			// net with source src_id does not exist; create it 
			// now first time;
			_nets.push_back( NET(_nets_count, name, src_id, type));
			_nets_count ++; // prepare for next net;
			//printf("\n New net %d %s \n", _nets_count-1, name.c_str());
			return &_nets[ _nets_count - 1];
		}
	}

	NODE *get_input( int id) {
		assert(id >= 0 && id < _inputs_count);	
		return &_inputs[ id]; 
	}
	NODE *get_output( int id) {
		assert(id >= 0 && id < _outputs_count);	
		return &_outputs[ id]; 
	}
	NODE *get_node( int id) {
		assert(id >= 0 && id < _nodes_count);	
		return &_nodes[ id]; 
	}
	NET *get_net( int id) {
		return &_nets[ id]; 
	}

	// construct graph;
	bool buid_graph_from_file( string filename);

	// debug;
	void print_graph( bool print_only_nets = true);
};

#endif
