//Graph_Imp.h


#ifndef GRAPH_IMP_H_   // if Graph_Imp.h hasn't been included yet
#define GRAPH_IMP_H_   // including the necessary files


#include <vector>
#include "Triplet.h"
#include "ngraph.hpp"


//! Defines a class for storing edges of the graph
/*!
	Contains the necessary data members and functions required to store edges of the graph
*/
class Graph_Imp
{

  public:

  	//! Vector of Triplet objects storing all the vertices of the graph
	std::vector<Triplet> vertices;

	//! NGraph object storing all the edges of the graph
	NGraph::Graph edges;

	//! Constructor of class Graph_Imp
	Graph_Imp(){}


};

#endif 