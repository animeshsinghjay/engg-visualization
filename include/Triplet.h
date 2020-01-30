//Triplet.h


#ifndef TRIPLET_H_   // if Triplet.h hasn't been included yet
#define TRIPLET_H_   // including the necessary files


//! Defines a class for storing triplets
/*!
	Contains the necessary data members and functions required to store triplets
*/
struct Triplet
{

	//! First member of the triplet
	double one;

	//! Second member of the triplet
	double two;

	//! Third member of the triplet
	double three;

	//! Defines a function for the == (equate) operator
	bool operator== (const Triplet&ref) const {
	  	return (ref.one == one)&&(ref.two == two)&&(ref.three==three);
	}

};

#endif 