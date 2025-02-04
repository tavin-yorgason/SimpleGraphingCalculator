/*******************************************************************************
 * Graph.hpp
 * By Tavin Yorgason
 *
 * Graph class definition.
 * 
 * To do:
 *   - Make graph dynamic
 *   - Make equations[] a linked list
 ******************************************************************************/
#pragma once

#include "Equation.hpp"

// Graph size
const int GRAPH_WIDTH = 205; // in chars
const int GRAPH_HEIGHT = 93; // in chars

const int MAX_EQUATIONS = 10;

// Graph definition
class Graph
{
private:
	// MAKE DYNAMIC LATER
	char graph[ GRAPH_HEIGHT ][ GRAPH_WIDTH ];
	
	// Scale
	int yScale = 10;
	int xScale = 10;

	// Distance between ticks
	double deltaTick = 1;
	
	// Translation
	bool prevPtValid = false;
	int prevPt[2];
	int centerPt[2] = { ( GRAPH_WIDTH - 1 ) / 2,
						( GRAPH_HEIGHT - 1 ) / 2 };

	// For multiple equations on one graph
	int numEquations = 0;
	bool regraphing = false;
	PfEquation equations[ MAX_EQUATIONS ]; 

	// Functions
	void clearEquations();
	void setupGraph();

	double xPosToValue( int xPos );
	int yValToPos( double yVal );
	int xValToPos( double xVal );

	void plotPt( int xInt, int yInt );
	bool isPtOnGraph( int xInt, int yInt );
	void genVertLines( int xInt, int yInt );

	void print();

public:
	Graph();

	void zoom( double scalar, bool zoomTo );
	void resetGraph();
	void graphPfEquation( PfEquation equation );
};

