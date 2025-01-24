/*******************************************************************************
 * Graph.cpp
 * By Tavin Yorgason
 *
 * Graph function definitions.
 * 
 * To do:
 *  - Sometimes the last point shows up on the left side of the graph??
 ******************************************************************************/

#include <iostream>

#include "Graph.hpp"

using namespace std;

// Returns true if a char is an axis char
bool Graph::isAxisChar( char c )
{
	if( c == '|' || c == '-' || c == '+' )
	{
		return true;
	}
	return false;
}

// Prints the graph
void Graph::print()
{
	for( int r = 0; r < GRAPH_HEIGHT; r++ )
	{
		for( int c = 0; c < GRAPH_WIDTH; c++ )
		{
			cout << graph[r][c];
		}
		cout << '\n';
	}
}

// Graphs a postfix equation
void Graph::graphPfEquation( PfEquation equation )  // PUBLIC
{
	// Clear the graph and add axis
	setupGraph();

	// Loop through every column including 1 column off-screen each side
	for( int xPos = -1; xPos < GRAPH_WIDTH + 1; xPos++ )
	{
		// Get the x value from its position
		double xVal = xPosToValue( xPos );
//		cout << "xVal: " << xVal << "\n";
		
		// Solve the equation with the x value and convert to y position
		bool validResult = true;
		double yVal = equation.solve( xPosToValue( xPos ), validResult );
		if( validResult )
		{
			int yPos = yValToPos( yVal );
		
			// Plot the point
			plotPt( xPos, yPos );
//			plotPt( xPos + 2, yPos ); // DEBUG
			
			// Generate vertical lines (skip first point)
			if( xPos != -1 )
			{
				genVertLines( xPos, yPos );
			}
			prevPt[0] = xPos;
			prevPt[1] = yPos;
		}
	}

	print();
}

// Converts an x position in the graph array to an x value
double Graph::xPosToValue( int xPos )
{
	// Center value on char position
	double xVal = xPos + 0.5;

	// Translate
	xVal -= GRAPH_WIDTH / 2.0;

	// Scale
	xVal *= (double) X_SCALE / GRAPH_WIDTH;

	return xVal;
}

// Converts a y value to its y position on the graph array
int Graph::yValToPos( double yVal )
{
	// Scale to graph size
	yVal *= ( (double) GRAPH_HEIGHT / Y_SCALE );
	
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

// Plots a point on the graph given its indexes in the 2d array
void Graph::plotPt( int xPos, int yPos )
{
//	cout << "Is (" << xPos << ", " << yPos << ") on the graph?\n\n";
	
	// Only plot point if its inside the graph
	if( isPtOnGraph( xPos, yPos ) )
	{
		// Place point on graph
		char *point = &graph[ yPos ][ xPos ];

		if( isAxisChar( *point ) || *point == '0' )
		{
			*point = '0';
		}
		else
		{
			*point = 'O';
		}
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

// Clears the graph and adds axis
void Graph::setupGraph()
{
	// The -1 from height and width are to account for indices starting at 0
	centerPt[0] = ( GRAPH_WIDTH - 1 ) / 2;
	centerPt[1] = ( GRAPH_HEIGHT - 1 ) / 2;

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
		graph[r][ ( GRAPH_WIDTH - 1 ) / 2 ] = '|';
	}
	
	// x axis
	for( int c = 0; c < GRAPH_WIDTH; c++ )
	{
		graph[ ( GRAPH_HEIGHT - 1 ) / 2 ][c] = '-';
	}

	// Center point
	graph[ centerPt[1] ][ centerPt[0] ] = '+';
}

