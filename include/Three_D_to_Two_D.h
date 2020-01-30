//Three_D_to_Two_D.h

/*

Input File Format:

1 					int int int   			: Front viewing direction
2 					int int int   			: Top viewing direction
3 					int int int   			: Cutting Plane
4 					int v 					: Number of vertices
5					int int int 			: Coordinates of vertices (space separated x y z coordinates; each in a new line)
.
.
.
5+v-1
5+v 				int e 					: Number of edges
5+v+1				int int 	 			: Edges in xy projection (space separated start & end vertex; each in a new line)
.
.
.
5+v+e-1

*/


#ifndef THREE_D_TO_TWO_D_H_   // if Edges.h hasn't been included yet
#define THREE_D_TO_TWO_D_H_   // including 

#include <vector>
#include "Graph_Imp.h"
#include "Edges.h"
#include "Triplet.h"
#include <fstream>
//! Class for doing all operations on 3D to 2D
/*!
	This class contains all the functions and intermediate objects needed to tranform the object.
*/
class Three_D_to_Two_D
{

public:
	
	Three_D_to_Two_D(){}
	//! This is the object which has been input by the user.
	Graph_Imp G;
	//! This is the graph after projection on x-y plane.
	Graph_Imp projected_xy;
	//! This is the graph after projection on y-z plane.
	Graph_Imp projected_yz;
	//!This is the graph after projection on x-z plane.
	Graph_Imp projected_zx;
	//! This is the graph after rotation according to the direction ratios input by the user.
	Graph_Imp rotatedG; 
	//! This the graph after its isometric projection.
	Graph_Imp projected_isometric;
	//! A vector for storing all the faces of the 3D graph given by the user.
	std::vector<std::vector<Edge>> faces;
	//! Vector for storing all the hidden edges in the xy plane
	std::vector<Edge> hidden_xy;
	//! Vector for storing all the hidden edges in the yz plane
	std::vector<Edge> hidden_yz;
	//! Vector for storing all the hidden edges in the xz plane
	std::vector<Edge> hidden_zx;
	//! Vector for storing the hidden edges after isometric projection
	std::vector<Edge> hidden_isometric;
	//! This function takes as input the filename and returns a graph described by that input file.
	/*!
		\param f This is the string filename of the file which contains the input graph specified by the user.
	*/
	Graph_Imp toGraph(char* f);
	//! Function for taking the isometric projection of the graph.
	/*!
		\param The input graph which needs to be projected isometrically.
		\sa projected_isometric
	*/
	Graph_Imp Projection_isometric(Graph_Imp g);
	//! Function which checks if a vertice is present on a face or not.
	/*!
		\param vert Defines the vertice to be checked
		\param face Gives the face which has to be checked
	*/
	bool vertOnFace(int vert, std::vector<Edge> face);
	//! Checks if a vertice is outside 
	/*
		\param xp x-coordinate of the point to be checked
		\param yp y-coordinate of the point to be checked
		\param face The face for which this has to be checked
		\param Original set of vertices to find the coordinates of the vertices correspoding to the edge in the face
	*/
	bool vertOutsideFace(double xp, double yp, std::vector<Edge> face, std::vector<Triplet> vert);
	bool vertOutsideFace_yz(double xp, double yp, std::vector<Edge> face, std::vector<Triplet> vert);
	bool vertOutsideFace_zx(double xp, double yp, std::vector<Edge> face, std::vector<Triplet> vert);
	//! Checks if the vertice is behind a face.
	/*!
		Compares the third coordinate to infer if the vertice is behind a face or not.
		\param zp the thord coordinate
		\param face The face to be checked
		\param vert The orginal vertice vector to check the third coordinate.
	*/
	bool vertBehindFace(double zp, std::vector<Edge> face, std::vector<Triplet> vert);
	bool vertBehindFace_yz(double zp, std::vector<Edge> face, std::vector<Triplet> vert);
	bool vertBehindFace_zx(double zp, std::vector<Edge> face, std::vector<Triplet> vert);
	//! Finds an edge in a vector of edges.
	/*!
		\param a The edge to be found
		\param hidden The vector to be checked
	*/
	bool findEdge(Edge a, std::vector<Edge> hidden);
	//! Function for taking the xy projection of the graph.
	/*!
		\param The input graph which needs to be projected isometrically.
		\sa projected_xy
	*/
	Graph_Imp Projectionxy(Graph_Imp g);
	//! Function for taking the yz projection of the graph.
	/*!
		\param The input graph which needs to be projected isometrically.
		\sa projected_yz
	*/
	Graph_Imp Projectionyz(Graph_Imp g);
	//! Function for taking the zx projection of the graph.
	/*!
		\param The input graph which needs to be projected isometrically.
		\sa projected_zx
	*/
	Graph_Imp Projectionzx(Graph_Imp g);
	//! Function for rotating a vector of vertices/
	/*!
		Defining the vertices.
	*/
	std::vector<Triplet> rotate_vector(std::vector<Triplet> vertices, Triplet normal_plane);


};


#endif 