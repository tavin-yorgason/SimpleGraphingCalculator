/*******************************************************************************
 * main.cpp
 *
 * Graphing calculator lol
 * By Tavin Yorgason
 * 
 * To do:
 *	 - Add commands to change graph variables (must also make graph dynamic).
 *     - Translate
 *     - Scale
 *       - Uniform scaling
 *       - Scale x/y separately
 *     - Auto
 *   - Add error handling to Equation constructors.
 *   - Add protections against converting infToPf() more than once.
 *   - Make command to switch between radian and degree mode.
 *   - Consider changing for loops to use size_t instead of int.
 *
 ******************************************************************************/

#include <iostream>
#include <string>

#include "Graph.hpp"

// Also includes PfEquation and InfEquation
#include "Equation.hpp"

using namespace std;

/**************/
/* Prototypes */
/**************/

// Convert a string to lowercase
void toLower( string &str );

// List commands
void printCommands();

/********/
/* Main */
/********/

// Manage input
int main()
{
	// This is what would happen if "graphing calculator" and "tavin" had a
	// really ugly baby.
	cout << "GRAVINCULATOR\n"
		 << "Use ~ instead of - for negative numbers\n"
		 << "Type 'help' for a list of commands\n\n";

	// Setup
	Graph graph;
	string input;
	
	// Get input
	cout << "> ";
	getline( cin, input );
	toLower( input );

	// Input loop
	while( input != "quit" && input != "exit" )
	{
		int inputLen = input.length();

		// Find command
		if( input == "help" )
		{
			printCommands();
		}
		else if( input == "new" )
		{
			graph.resetGraph();
			cout << "Graph reset\n";
		}
		else if( input == "plot" )
		{
			// Get equation
			cout << "y = ";
			getline( cin, input );
			
			// Convert input to a postfix equation
			InfEquation temp( input );
			PfEquation equation( temp.infixToPostfix() );
			cout << "Postfix equation: " << equation.getEquation() << endl;

			// Graph the result
			graph.graphPfEquation( equation );
			cout << "\n";
		}
		else if( inputLen > 5 && input.compare( 0, 5, "zoom " ) == 0 )
		{
			double multiplier = stod( input.substr( 5, inputLen - 5 ) );
			graph.zoom( multiplier, false );
		}
		else if( inputLen > 7 && input.compare( 0, 7, "zoomto " ) == 0 )
		{
			double newScale = stod( input.substr( 7, inputLen - 7 ) );
			graph.zoom( newScale, true );
		}
		else
		{
			cout << "ERROR: Invalid command.\n";
		}
		
		// Get input
		cout << "> ";
		getline( cin, input );
		toLower( input );
	}

	// Exit
	cout << "\nThanks for trying out my calculator!\n";
}

/*************/
/* Functions */
/*************/

// Convert a string to lowercase
void toLower( string &str )
{
	for( int i = 0; i < str.length(); i++ )
	{
		if( 'A' <= str[i] && str[i] <= 'Z' )
		{
			cout << str[i] << endl;
		}
	}
}

// List commands
void printCommands()
{
	cout << "COMMANDS\n"
		 << " - 'exit': Exits the program\n"
		 << " - 'new': Creates a new graph\n"
		 << " - 'plot': Prompts for an equation to plot on the graph\n"
		 << " - 'zoom X.X': Zooms in or out by the amount specified\n\n";
}

