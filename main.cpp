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

#include "Graph.hpp"

// Also includes PfEquation and InfEquation
#include "Equation.hpp"

using namespace std;

int main()
{
	// This is what would happen if "graphing calculator" and "tavin" had a
	// really ugly baby.
	cout << "Gravinculator\n"
		 << "Type \"exit\" to exit\n"
		 << "Use ~ instead of - for negative numbers\n\n";

	// Input loop
	string input;
	cout << "Enter an equation: y = ";
	getline( cin, input );
	while( input != "exit" )
	{
		// Convert input to a postfix equation
		InfEquation temp( input );
		PfEquation equation( temp.infixToPostfix() );
		cout << "Postfix equation: " << equation.getEquation() << endl;

		// Graph the result
		Graph graph;
		
		cout << "Graph:\n";
		graph.graphPfEquation( equation );
		cout << "\n";

		// Get input
		cout << "Enter an equation: y = ";
		getline( cin, input );
	}

	// Exit
	cout << "\nThanks for trying out my calculator!\n";
}

