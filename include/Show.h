//Show.h



#ifndef SHOW_H_   // if Show.h hasn't been included yet
#define SHOW_H_   // including 

#define WIDTH 700
#define HEIGHT 700

class Three_D_to_Two_D;

#include <vector>
#include "Graph_Imp.h"
#include "Triplet.h"
#include "Edges.h"
#include <QtCore>
#include <QtGui>

//! This is the class which will be used to handle all function related to showing and rendering the graph.
class Show
{

public:

	Show(){}
	//! Function to scale the graph given to it as an input
	/*!
		\param g The input graph to be scaled.
		\param mode The mode (xy,yz,zx) in which the current scaling has to be done
	*/
	Graph_Imp set_acc_to_ranges(Graph_Imp g, int mode);
	//! A function to find an edge in a scaled vector
	bool findEdgeInVector(Edge a, std::vector<Edge> hidden);
	//! Draw the input 2D graph on the QtCanvas
	/*!
		\param g The graph to be shown.
		\param p The painter to be used
		\param hidden the hidden edges for the current graph.
		\param hide Do you want to enable hiding
	*/
	void drawGraph(Graph_Imp g, QPainter &p, std::vector<Edge> hidden, bool hide);
	//! Main function which takes the 3DtoTwoD object as an input and makes the projections.
	/*!
		/param T The object to be projected
		/param topdir The direction ratios for rotation
	*/
	int show_qt_projections(Three_D_to_Two_D &T, Triplet topdir);

};


#endif 