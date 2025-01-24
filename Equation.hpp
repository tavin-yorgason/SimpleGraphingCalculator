/*******************************************************************************
 * Equation.hpp
 * By Tavin Yorgason
 *
 * Class definitions for Equation classes.
 * 
 * To do:
 *  - Auto detect the variable for an equation.
 ******************************************************************************/
#pragma once

#include <string>
#include <stack>

// Base abstract equation class
class Equation
{
private:
	int precedence( std::string op );

protected:
	std::string equation;

public:
	std::string getEquation() { return equation; }
	bool isOperator( char c );
	bool isNum( char c );
	bool isNumStart( char c );
	bool isVariable( char c );
	bool isFunction( std::string function );
	bool hasPrecedence( std::string op1, std::string op2 );
	double performOperation( char op, double term1, double term2 );
};


// Infix equation class
class InfEquation : public Equation
{
private:
/*	void printStack( std::stack<char> &myStack )
	{
		std::stack<char> buffer;

		while( !myStack.empty() )
		{   
			buffer.push( myStack.top() );
//			std::cout << myStack.top() << std::endl;
			myStack.pop();
		}   
		while( !buffer.empty() )
		{   
			myStack.push( buffer.top() );
			buffer.pop();
		}   
	}*/
	
	bool isLetter( char c );
	bool checkImplyMultiply( int startIndex );
//	std::string equation;
	
//	void cleanup();

public:
	// LATER ADD ERROR HANDLING TO CONSTRUCTOR
	InfEquation( std::string equationIn ) { equation = equationIn; }
	~InfEquation();


	std::string infixToPostfix();
//	void errorCheck();
};


// Postfix equation class
class PfEquation : public Equation
{
private:
	std::string replaceVar( double varVal );
	double evalFunction( std::string function, double num );

public:
	// LATER ADD ERROR HANDLING TO CONSTRUCTOR
	PfEquation( std::string equationIn ) { equation = equationIn; }
	~PfEquation();

	
	double solve( double x, bool &validResult );
};

