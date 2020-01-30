#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include "../include/ngraph.hpp"
#include "../include/gnuplot_i.hpp"
#include "../include/Two_D_to_Three_D.h"

using namespace NGraph;
using namespace std;


void Two_D_to_Three_D::wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}


// struct Triplet
// {
//   double  one, two, three;
//   bool operator== (const Triplet&ref) const {
//   	return (ref.one == one)&&(ref.two == two)&&(ref.three==three);
//   }
// };

// class Graph_Imp
// {
// public:
// 	vector<Triplet> vertices;
// 	Graph edges;
// 	Graph_Imp(){

// 	}
// };



void Two_D_to_Three_D::toGraphAllThree(char* f, Graph_Imp &x_g, Graph_Imp &y_g, Graph_Imp &z_g)
{
	Graph A,B,C;
	ifstream afile;
	afile.open(f);
	double x,y,z;
	int e,n;
	afile>>n;
	std::vector<Triplet> vert;
	for (int i = 0; i < n; ++i)
	{
		afile>>x>>y>>z;
		Triplet temp = {x,y,z};
		vert.push_back(temp);
	}
	afile>>e;
	for (int i = 0; i < e; ++i)
	{
		afile>>x>>y;
		if (x<y) A.insert_edge(x,y);
		else A.insert_edge(y,x);
	}
	x_g.vertices = vert;
	x_g.edges = A;
	vert.clear();
	for (int i = 0; i < n; ++i)
	{
		afile>>x>>y>>z;
		Triplet temp = {x,y,z};
		vert.push_back(temp);
	}
	afile>>e;
	for (int i = 0; i < e; ++i)
	{
		afile>>x>>y;
		if (x<y) B.insert_edge(x,y);
		else B.insert_edge(y,x);
	}
	y_g.vertices = vert;
	y_g.edges = B;
	vert.clear();
	for (int i = 0; i < n; ++i)
	{
		afile>>x>>y>>z;
		Triplet temp = {x,y,z};
		vert.push_back(temp);
	}
	afile>>e;
	for (int i = 0; i < e; ++i)
	{
		afile>>x>>y;
		if (x<y) C.insert_edge(x,y);
		else C.insert_edge(y,x);
	}
	z_g.vertices = vert;
	z_g.edges = C;
	afile.close();
	
}



std::vector<Triplet> Two_D_to_Three_D::make3DVertices(Graph_Imp &x, Graph_Imp &y, Graph_Imp &z){
	std::vector<Triplet> vert;
	for (int i = 0; i < x.vertices.size(); ++i)
	{
		Triplet temp = x.vertices.at(i);
		temp.three = y.vertices.at(i).three;
		vert.push_back(temp);
	}
	return vert;
}




std::vector<int> Two_D_to_Three_D::sameCoordinates(std::vector<Triplet> vert, Triplet toBeFound){
	std::vector<int> ret;
	for (int i = 0; i < vert.size(); ++i)
	{
		if (vert.at(i) == toBeFound)
			ret.push_back(i);
	}
	return ret;
}




std::vector< pair<int,int> > Two_D_to_Three_D::makeEdges(Graph_Imp &x){
	std::vector< pair<int,int> > pEdges;
	std::vector<int> u,v;
	for (int i = 0; i < x.vertices.size(); ++i)
	{	
		Graph::vertex_set S = x.edges.out_neighbors(i);
		u = sameCoordinates(x.vertices, x.vertices.at(i));
		for (Graph::vertex_set::const_iterator p = S.begin(); p != S.end(); ++p)
		{
			v = sameCoordinates(x.vertices, x.vertices.at(*p));
			// cout<<i<<" "<<*p<<endl;
			// for (int i = 0; i < u.size(); ++i)
			// {
			// 	cout<<u[i]<<" ";
			// }
			// for (int i = 0; i < v.size(); ++i)
			// {
			// 	cout<<v[i]<<" ";
			// }
			// cout<<endl;
			for (int i = 0; i < u.size(); ++i)
			{
				for (int j = 0; j < v.size(); ++j)
				{
					pEdges.push_back(make_pair(u.at(i), v.at(j)));
					pEdges.push_back(make_pair(v.at(j), u.at(i)));
				}
			}
			for (int i = 0; i < u.size(); ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					pEdges.push_back(make_pair(u.at(i), u.at(j)));
					pEdges.push_back(make_pair(u.at(j), u.at(i)));
				}
			}
			for (int i = 0; i < v.size(); ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					pEdges.push_back(make_pair(v.at(i), v.at(j)));
					pEdges.push_back(make_pair(v.at(j), v.at(i)));
				}
			}
			// for (int i = 0; i < pEdges.size(); ++i)
			// {
			// 	cout<<pEdges[i].first<<" "<<pEdges[i].second<<" ";
			// }
			// cout<<endl;
		}
	}
	return pEdges;
}




std::vector<pair<int, int> > Two_D_to_Three_D::intersection(std::vector<pair<int, int> > a, std::vector<pair<int, int> > b, std::vector<pair<int, int> > c){
	std::vector<pair<int, int> > ret;
	bool inb, inc = false;
	for (int i = 0; i < a.size(); ++i)
	{
		inb = false;
		inc = false;
		for (int j = 0; j < b.size(); ++j)
		{
			if (a[i] == b[j]){
				inb = true;
				break;
			}
		}
		for (int j = 0; j < c.size(); ++j)
		{
			if (a[i] == c[j]){
				inc = true;
				break;
			}
		}
		if (inb && inc){
			ret.push_back(a[i]);
		}
	}
	return ret;
}





Graph_Imp Two_D_to_Three_D::makethreed(Graph_Imp G_on_xy, Graph_Imp G_on_yz, Graph_Imp G_on_xz){
	Graph_Imp G3D;
	G3D.vertices = make3DVertices(G_on_xy, G_on_yz, G_on_xz);
	std::vector< pair<int,int> > probEdges_xy = makeEdges(G_on_xy);
	std::vector< pair<int,int> > probEdges_yz = makeEdges(G_on_yz);
	std::vector< pair<int,int> > probEdges_xz = makeEdges(G_on_xz);
	std::vector<pair<int, int> > inter = intersection(probEdges_xy, probEdges_yz, probEdges_xz);
	for (int i = 0; i < inter.size(); ++i)
	{
		G3D.edges.insert_edge(inter[i].first, inter[i].second);
		G3D.edges.insert_edge(inter[i].second, inter[i].first);
	}
	return G3D;
}



void Two_D_to_Three_D::show_gnu_plot(Graph_Imp G3D){
	ofstream ofile;
	ofile.open("out.txt");
	for (int i = 0; i < G3D.vertices.size(); ++i)
	{
		Graph::vertex_set S = G3D.edges.out_neighbors(i);
		for (Graph::vertex_set::const_iterator p = S.begin(); p != S.end(); ++p) {
			ofile<<G3D.vertices[i].one<<" "<<G3D.vertices[i].two<<" "<<G3D.vertices[i].three<<endl;
			ofile<<G3D.vertices[*p].one<<" "<<G3D.vertices[*p].two<<" "<<G3D.vertices[*p].three<<"\n\n\n";
		}
	}
	ofile.close();
	Gnuplot g;
	g<<"unset xtics";
	g<<"unset ytics";
	g<<"unset ztics";
	g<<"unset border";
	g<<"set terminal x11 title '3D object formed'";
	g<<"splot 'out.txt' with lines notitle";
	wait_for_key();
}






// int main(int argc, char const *argv[])
// {
// 	Graph_Imp G_on_xy,G_on_yz,G_on_xz;
// 	toGraphAllThree("inp2.txt",G_on_xy,G_on_yz,G_on_xz);
// 	Graph_Imp G3D = makethreed(G_on_xy, G_on_yz, G_on_xz);
// 	// G3D.vertices = make3DVertices(G_on_xy, G_on_yz, G_on_xz);
// 	// std::vector< pair<int,int> > probEdges_xy = makeEdges(G_on_xy);
// 	// // cout<<"---------"<<endl;
// 	// std::vector< pair<int,int> > probEdges_yz = makeEdges(G_on_yz);
// 	// // cout<<"---------"<<endl;
// 	// std::vector< pair<int,int> > probEdges_xz = makeEdges(G_on_xz);
// 	// // cout<<"---------"<<endl;
// 	// std::vector<pair<int, int> > inter = intersection(probEdges_xy, probEdges_yz, probEdges_xz);
// 	// // cout<<"---------"<<endl;
// 	// for (int i = 0; i < inter.size(); ++i)
// 	// {
// 	// 	G3D.edges.insert_edge(inter[i].first, inter[i].second);
// 	// 	G3D.edges.insert_edge(inter[i].second, inter[i].first);
// 	// }
// 	// cout<<inter.size()/2;
// 	// for (int i = 0; i < G3D.vertices.size(); ++i)
// 	// {	
// 	// 	cout<<G3D.vertices[i].one<<" "<<G3D.vertices[i].two<<" "<<G3D.vertices[i].three<<endl;	
// 	// }
// 	// cout<<"------------"<<endl;
// 	// cout<<G3D.edges;
// 	show_gnu_plot(G3D);
// }
