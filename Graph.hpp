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
const int GRAPH_WIDTH = 189; // in chars
const int GRAPH_HEIGHT = 85; // in chars

// Graph definition
class Graph
{
private:
	// MAKE DYNAMIC LATER
	char graph[ GRAPH_HEIGHT ][ GRAPH_WIDTH ];
	
	// For multiple equations on one graph
	int numEquations = 0;

	// Functions
	double xPosToValue( int xPos );
	int yValToPos( double yVal );
	
	void plotPt( int xInt, int yInt );
	bool isPtOnGraph( int xInt, int yInt );
	int prevPt[2];
	int centerPt[2];
	void genVertLines( int xInt, int yInt );

//	bool isAxisChar( char c );

	void print();

public:
	Graph();
	void setupGraph();
	void graphPfEquation( PfEquation equation );
};

