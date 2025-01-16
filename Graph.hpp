/*******************************************************************************
 * Graph.hpp
 * By Tavin Yorgason
 *
 * Graph class definition.
 * 
 ******************************************************************************/
#pragma once

#include "Equation.hpp"

// Graph variables
const int X_SCALE = 5;
const int Y_SCALE = 5;
const int GRAPH_WIDTH = 191; // in chars
const int GRAPH_HEIGHT = 87; // in chars

// Graph definition
class Graph
{
private:
	// MAKE DYNAMIC LATER
	char graph[ GRAPH_HEIGHT ][ GRAPH_WIDTH ];
	
	double xPosToValue( int xPos );
	int yValToPos( double yVal );
	
	void plotPt( int xInt, int yInt );
	bool isPtOnGraph( int xInt, int yInt );
	int prevPt[2];
	int centerPt[2];
	void genVertLines( int xInt, int yInt );

	void setupGraph();
	bool isAxisChar( char c );

	void print();

public:
	void graphPfEquation( PfEquation equation );
};

