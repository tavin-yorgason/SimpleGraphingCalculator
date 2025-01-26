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
         - Scale x/y separately
 *     - Auto
 *   - Add error handling to Equation constructors.
 *   - Add protections against converting infToPf() more than once.
 *   - Make command to switch between radian and degree mode.
 *   - Allow multiple equations on the screen at once.
 *   - 
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
	while( input != "quit" )
	{
		if( input == "help" )
		{
			printCommands();
		}
		else if( input == "new" )
		{
			graph.setupGraph();
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
			cout << "Graph:\n";
			graph.graphPfEquation( equation );
			cout << "\n";
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
		 << " - 'plot': Prompts for an equation to plot on the graph\n\n";
}

