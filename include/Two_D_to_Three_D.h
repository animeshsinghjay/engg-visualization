//Two_D_to_Three_D.h

/*

Input File Format:

1						int v		    			: Number of vertices (is a constant through all the projections)
2 						int int int		    		: Coordinates of vertices in xy projection (space separated x y z coordinates; each in a new line)
.
.
2+v-1 
2+v 					int exy						: Number of edges in xy projection
2+v+1 					int int 	 				: Edges in xy projection (space separated start & end vertex; each in a new line)
.
.
2+v+exy-1
2+v+exy 				int int int					: Coordinates of vertices in yz projection (space separated x y z coordinates; each in a new line)
.
.
2+v+exy+v-1				
2+v+exy+v 				int eyz 					: Number of edges in yz projection
2+v+exy+v+1 			int int 					: Edges in yz projection (space separated start & end vertex; each in a new line)
.
.
2+v+exy+v+eyz-1
2+v+exy+v+eyz			int int int					: Coordinates of vertices in zx projection (space separated x y z coordinates; each in a new line)
.
.
2+v+exy+v+eyz+v-1				
2+v+exy+v+eyz+v 		int ezx 					: Number of edges in zx projection
2+v+exy+v+eyz+v+1 		int int 					: Edges in zx projection (space separated start & end vertex; each in a new line)
.
.
2+v+exy+v+eyz+v+ezx-1


*/


#ifndef TWO_D_TO_THREE_D_H_   // if Two_D_to_Three_D.h hasn't been included yet
#define TWO_D_TO_THREE_D_H_   // including the required files


#include <vector>
#include "Triplet.h"
#include "Graph_Imp.h"


//! Defines a class needed to generate the 3D object from the 3 2D projections
/*!
	Contains the necessary data members and functions required to generate the 3D object from the 3 2D projections
*/
class Two_D_to_Three_D
{

  public:

  	//! Defines a contructor for the class Two_D_to_Three_D
	Two_D_to_Three_D(){}

	//! Waits for user input
	/*!
		Gtk Window tends to close on lack of user input <br>
		It prevents the GUI window from closing and helps in waiting for the user's response
	*/
	void wait_for_key ();

	//! Makes the 3 graphs which have been input by the user
	/*!
		\param f Takes the file as an input
		\param x_g Makes the xy graph
		\param y_g Makes the yz graph
		\param z_g Makes the xz graph
	*/
	void toGraphAllThree(char* f, Graph_Imp &x_g, Graph_Imp &y_g, Graph_Imp &z_g);

	//! Makes 3D vertices
	/*!
		Takes the 3 2D vertex inputs and converts them to 3D vertices
		\param x The first graph
		\param y The second graph
		\param z The thord graph
	*/
	std::vector<Triplet> make3DVertices(Graph_Imp &x, Graph_Imp &y, Graph_Imp &z);

	//! Checks if 2 vertices have a common coordinate
	/*!
		\param vert The vector of vertices
		\param toBeFound The triplet to be found
	*/
	std::vector<int> sameCoordinates(std::vector<Triplet> vert, Triplet toBeFound);

	//! Makes the edges of the final 3D graph
	/*!
		\param x The graph to maje edges for
	*/
	std::vector<std::pair<int,int> > makeEdges(Graph_Imp &x);

	//! Takes three vectors as an input and returns their intersection/
	std::vector<std::pair<int, int> > intersection(std::vector<std::pair<int, int> > a, std::vector<std::pair<int, int> > b, std::vector<std::pair<int, int> > c);
	
	//! The final 3D graph is made using this function.
	/*!
		Takes the 3 input graphs and converts it into a single 3D graph.
	*/
	Graph_Imp makethreed(Graph_Imp G_on_xy, Graph_Imp G_on_yz, Graph_Imp G_on_xz);

	//! Function to show the 3D graph.
	/*!
		\param G3D The 3d graph to be shown as a wireframe output.
	*/
	void show_gnu_plot(Graph_Imp G3D);

};


#endif 