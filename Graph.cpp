/*******************************************************************************
 * Graph.cpp
 * By Tavin Yorgason
 *
 * Graph function definitions.
 * 
 * To do:
 *  - Move the viewing window around
 *  - Add '+'s to mark coordinates
 *  - Instead of calculating the x value every time, calculate the x distance
 *    between two points.
 ******************************************************************************/

#include <iostream>
#include <cmath>

#include "Graph.hpp"

using namespace std;

// Constructor
Graph::Graph()
{
	// Clear the graph and add axis
	setupGraph();
}

// Scales the graph and reprints it
void Graph::scale( double multiplier )
{
	regraphing = true;

	// Change scale and clear graph
	xScale *= multiplier;
	yScale *= multiplier;
	setupGraph();

	// Replot all equations
	int numEquationsSave = numEquations;
	numEquations = 0;
	for( int i = 0; i < numEquationsSave; i++ )
	{
		graphPfEquation( equations[i].getEquation() );
	}

	// Print new graph
	print();
	
	regraphing = false;
}

// Prints the graph
void Graph::print()
{
	cout << "Distance between tick marks: " << deltaTick << "\n";
	for( int r = 0; r < GRAPH_HEIGHT; r++ )
	{
		for( int c = 0; c < GRAPH_WIDTH; c++ )
		{
			char gChar = graph[r][c];
			
			// Change color of text for different lines
			if( '1' <= gChar && gChar <= '9' )
			{
				// Color loops through red, green, yellow, blue, magenta, cyan,
				// and white
				int color = 31 + ( gChar - '0' - 1 ) % 7;
				cout << "\033[" << color << "mo\033[0m";
			}
			else
			{
				cout << gChar;
			}
	
		}
		cout << '\n';
	}
}

// Graphs a postfix equation
void Graph::graphPfEquation( PfEquation equation )  // PUBLIC
{
	// Return if too many equations
	if( numEquations == MAX_EQUATIONS - 1 )
	{
		cout << "ERROR: Too many equations.\n";
		return;
	}

	// Add equation to equations list if not regraphing
	if( !regraphing )
	{
		equations[ numEquations ] = equation;
	}
	numEquations++;

	// Loop through every column including 1 column off-screen each side
	for( int xPos = -1; xPos < GRAPH_WIDTH + 1; xPos++ )
	{
		// Solve the equation with the x value and convert to y position
		bool validResult = true;
		double yVal = equation.solve( xPosToValue( xPos ), validResult );
		if( validResult )
		{
			int yPos = yValToPos( yVal );
		
			// Plot the point
			plotPt( xPos, yPos );
//			plotPt( xPos + 2, yPos ); // DEBUG
			
			// Generate vertical lines
			if( prevPtValid )
			{
				genVertLines( xPos, yPos );
			}
			prevPt[0] = xPos;
			prevPt[1] = yPos;
		}
		prevPtValid = validResult;
	}

	// Print result
	if( !regraphing )
	{
		print();
	}
}

// Converts an x position in the graph array to an x value
double Graph::xPosToValue( int xPos )
{
	// Center value on char position
	double xVal = xPos + 0.5;

	// Translate
	xVal -= GRAPH_WIDTH / 2.0;

	// Scale
	xVal *= (double) xScale / GRAPH_WIDTH;

	return xVal;
}

// Converts a y value to its y position on the graph array
int Graph::yValToPos( double yVal )
{
	// Scale to graph size
	yVal *= ( (double) GRAPH_HEIGHT / yScale );
	
	// Round to nearest int
	int yPos;
	if( yVal > 0 )  // positive
	{
		yPos = yVal + 0.5;
	}
	else            // negative
	{
		yPos = yVal - 0.5;
	}

	// Translate to correct position
	yPos = GRAPH_HEIGHT / 2 - yPos;

	return yPos;
}

// Converts a x value to its x position on the graph array
int Graph::xValToPos( double xVal )
{
	// Scale to graph size
	xVal *= ( (double) GRAPH_WIDTH / xScale );
	
	// Round to nearest int
	int xPos;
	if( xVal > 0 )  // positive
	{
		xPos = xVal + 0.5;
	}
	else            // negative
	{
		xPos = xVal - 0.5;
	}

	// Translate to correct position
	xPos = xPos - GRAPH_WIDTH / 2;

	return xPos;
}

// Plots a point on the graph given its indexes in the 2d array
void Graph::plotPt( int xPos, int yPos )
{
//	cout << "Is (" << xPos << ", " << yPos << ") on the graph?\n\n";
	
	// Only plot point if its inside the graph
	if( isPtOnGraph( xPos, yPos ) )
	{
		// Place point on graph
		char *point = &graph[ yPos ][ xPos ];
		*point = numEquations + '0';
	}
}

// Checks if a point is on the graph
bool Graph::isPtOnGraph( int xPos, int yPos )
{
//	cout << "Is (" << xPos << ", " << yPos << ") on the graph?\n\n";
	
	if( ( 0 <= yPos && yPos < GRAPH_HEIGHT )
	 && ( 0 <= xPos && xPos < GRAPH_WIDTH ) )
	{
		return true;
	}
	return false;
}

// Generates vertical lines so there aren't gaps in the line
void Graph::genVertLines( int xPos, int yPos )
{
	// if yDiff is positive, the graph is increasing between those points
	int yDiff = prevPt[1] - yPos;

	// Return if both curr and prev points are outside of the graph
	if( !isPtOnGraph( xPos, yPos ) && !isPtOnGraph( prevPt[0], prevPt[1] ) )
	{
//		cout << "No vertical line\n";
		return;
	}

	// Create vars for line
	int startY;  // inclusive
	int endY;  // exclusive
	int middleY;
	int startX;
	int endX;

	// Set vars for creating vertical line
	if( yDiff > 0 )  // positive slope
	{
		startY = yPos + 1;
		middleY = startY + yDiff / 2;
		endY = prevPt[1];
		
		startX = xPos;
		endX = startX - 1;
	}
	else  // negative slope
	{
		startY = prevPt[1] + 1;
		middleY = startY - yDiff / 2;
		endY = yPos;
		
		startX = prevPt[0];
		endX = startX + 1;
	}

	// Fixes symmetry issues along y-axis
	if( yPos < centerPt[1] && yDiff % 2 == 0 ) // y is positive and yDiff is even
	{
		middleY--;
	}

	// Create vert line
	for( int i = startY; i < endY; i++)
	{
		if( i >= middleY )
		{
			plotPt( endX, i );
		}
		else
		{
			plotPt( startX, i );
		}
	}
}

// Clear equations
void Graph::clearEquations()
{
	for( int i = 0; i < numEquations; i++ )
	{
		equations[i].setEquation( "" );
	}
	numEquations = 0;
}

// Resets equations, but not scale
void Graph::resetGraph()
{
	setupGraph();
	clearEquations();
}

// Clears the graph and adds axis
void Graph::setupGraph()
{
	// Clear graph
	for( int r = 0; r < GRAPH_HEIGHT; r++ )
	{
		for( int c = 0; c < GRAPH_WIDTH; c++ )
		{
			graph[r][c] = ' ';
		}
	}

	// y axis
	for( int r = 0; r < GRAPH_HEIGHT; r++ )
	{
		graph[r][ centerPt[0] ] = '|';
	}
	
	// x axis
	for( int c = 0; c < GRAPH_WIDTH; c++ )
	{
		graph[ centerPt[1] ][c] = '-';
	}

	// Center point
	graph[ centerPt[1] ][ centerPt[0] ] = '+';

	// Grid marks
	// Update deltaTick (CHANGE TO HAVE deltaTickY AND deltaTickX)
//	cout << "yScale / deltaTick: " << yScale / deltaTick << "\n";
	while( yScale / deltaTick > 20 )
	{
		deltaTick *= 5;
//		cout << "Changed deltaTick to: " << deltaTick << "\n";
	}
	while( yScale / deltaTick < 4 )
	{
		deltaTick *= 0.2;
//		cout << "Changed deltaTick to: " << deltaTick << "\n";
	}

	// Y axis
	double tickStart = -deltaTick * floor( (yScale / 2.0) / deltaTick );
//	cout << "tickStart: " << tickStart << "\n";
	for( double i = tickStart; i <= -tickStart; i += deltaTick )
	{
		if( isPtOnGraph( centerPt[0], yValToPos(i) ) )
		{
			graph[ yValToPos(i) ][ centerPt[0] ] = '+';
			graph[ yValToPos(i) ][ centerPt[0] - 1 ] = '-';
			graph[ yValToPos(i) ][ centerPt[0] + 1] = '-';
		}
	}
	
	// X axis
	tickStart = -deltaTick * floor( (xScale / 2.0) / deltaTick );
	for( double i = tickStart; i <= -tickStart; i += deltaTick )
	{
		if( isPtOnGraph( centerPt[0], yValToPos(i) ) )
		{
			// Don't ask me why this needs to be offset, I cannot figure it out
			graph[ centerPt[1] + 1 ][ xValToPos(i) - 1 ] = '+';
			graph[ centerPt[1] ][ xValToPos(i) - 1 ] = '|';
			graph[ centerPt[1] + 2 ][ xValToPos(i) - 1 ] = '|';
		}
	}
}

