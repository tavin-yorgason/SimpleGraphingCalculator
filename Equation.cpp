/*******************************************************************************
 * Equation.cpp
 *
 * Member functions for Equation, PfEquation, and InfixEquation.
 * By Tavin Yorgason
 *
 * To do:
 *   - Fix math functions so that it allows functions that don't have 3 letters.
 *   - Allow '-' for neg nums instead of requiring '~'
 ******************************************************************************/

#include <iostream>
#include <string>
#include <stack>
#include <cmath>

#include "Equation.hpp"
using namespace std;

const char VARIABLE = 'x';

/************/
/* Equation */
/************/

// Returns true if c is an operator (+, -, *, /, ^, %)
bool Equation::isOperator( char c )
{
	if( c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' )
	{
		return true;
	}
	return false;
}

// Returns true if c is a number
bool Equation::isNum( char c )
{
	if( '0' <= c && c <= '9' )
	{
		return true;
	}
	return false;
}

// Returns true if the char is a letter
bool Equation::isLetter( char c )
{
	if( 'a' <= c && c <= 'z' )
	{
		return true;
	}
	return false;
}

// Returns true if op1 has precedence over op2
bool Equation::hasPrecedence( string op1, string op2 )
{
	if( precedence( op1 ) >= precedence( op2 ) )
	{
		return true;
	}
	return false;
}

// Returns the precendence level of the operator
int Equation::precedence( string op )  // PRIVATE
{
//	cout << "[precedence] op: " << op << endl;
	if( op == "(" )
	{
		return -1;
	}
	else if( op == "+" || op == "-" )
	{
		return 0;
	}
	else if( op == "*" || op == "/" || op == "%" )
	{
		return 1;
	}
	else if( op == "^" )
	{
		return 2;
	}
	else if( isFunction( op ) )
	{
		return 3;
	}
	
	cout << "[Equation::precedence] Invalid operator: "
		 << op << "\n";
	exit(1);
}

// Returns true if the location is the start of a number. ~ is the negative sign
bool Equation::isNumStart( char c )
{
	if( isNum( c ) || c == '.' || c == '~' )
	{
		return true;
	}
	return false;
}

// Returns true if the char is the chosen variable
bool Equation::isVariable( char c )
{
	if( c == VARIABLE || c == VARIABLE + ('A' - 'a') )
	{
		return true;
	}
	return false;
}

// Returns true if the string is the start of a function
bool Equation::isFunction( string function )
{
	if( function == "sqrt" )
	{
		return true;
	}
	else if( function == "cos" )
	{
		return true;
	}
	else if( function == "sin" )
	{
		return true;
	}
	else if( function == "tan" )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns the result of the calculation (move to PfEquation)
double Equation::performOperation( char op, double term1, double term2 )
{
	// No break statements needed
	switch( op )
	{
		case '+':
			return term1 + term2;
		case '-':
			return term1 - term2;
		case '*':
			return term1 * term2;
		case '/':
			return term1 / term2;
		// Modulo results in abnormal behavior when combined with the
		// Graph::genVertLines() function, will fix later.
		/*case '%':
		{
			// Custom modulo operation to work with doubles
			double mod = abs( term1 - (int) (term1 / term2) * term2 );

			// Ensure sign of term 2 is the sign of result
			if( ( term2 < 0 && mod > 0 ) || ( term2 > 0 && mod < 0 ) )
			{
				mod *= -1;
			}
			
			return mod;
		}*/
		case '^':
			return pow( term1, term2 );
		default:
			cout << "[Equation::performOperation] Invalid operator.\n";
			exit(1);
	}
}


/***************/
/* InfEquation */
/***************/

InfEquation::~InfEquation()
{
	
}

//string InfEquation::getEquation() { return equation; }


// Checks if equation[startIndex] is a character that can imply multiplication.
// If so, set the previous char to a *. Then, the loop that calls this function
// checks the char again and treats it like any normal operator.
bool InfEquation::checkImplyMultiply( int startIndex )  // PRIVATE
{
	// Skip spaces
	char c = equation[startIndex];
	int i = startIndex;
	while( i < equation.length() && c == ' ' )
	{
		i++;
		
		// Check for bounds
		if( i >= equation.size() )
		{
			return false;
		}

		c = equation[i];
	}

//	cout << "Does " << c << " imply multiplication? ";
	// Add * if it implies multiplication
	if( isNumStart( c ) || isLetter( c ) || c == '(' )
	{
		// EDITS INF EQUATION, MAKES IT UNUSEABLE
		equation[startIndex - 1] = '*';
		return true;
	}
//	cout << endl;
	return false;
}

// Convert an equation from infix to postfix. Deals with spaces.
string InfEquation::infixToPostfix()
{
	// Used for operators and parentheses
	stack<string> opStack;

	// Store result here
	string pfEquation;

	// Initiation (why?)
	opStack.push( "(" );
	equation += ')';

	// Loop through the infix equation and build the postfix version. Checks for
	// implied multiplication.
	for( int i = 0; i < equation.length(); i++ )
	{
//		cout << "Checking " << equation[i] << endl;
//		cout << "is it a numstart " << isNumStart( equation[i] ) << endl;

		/* ----- Parentheses ----- */
		if( equation[i] == '(' )
		{
			opStack.push( "(" );
		}
		else if( equation[i] == ')' )
		{
			// Put any operators that were between parentheses into the equation
//			cout << "Printing opStack and equation[i] = ')'\n";
//			printStack( opStack );
			while( opStack.top() != "(" )
			{
				pfEquation += opStack.top();
//				cout << "Adding " << opStack.top() << " to opStack\n";
				pfEquation += ' ';
//				cout << "pfEquation: " << pfEquation << endl;
				opStack.pop();
			}
			opStack.pop();
			
			// Check for implied multiplication
			if( checkImplyMultiply( i + 1 ) )
			{
				// checkImplyMultiply() places a * in the current index, so the
				// i-- forces this char to be checked again and then it's
				// treated as any operator is.
				i--;
			}
		}
		/* ----- Variables ----- */
		else if( isVariable( equation[i] )
			|| ( equation[i] == '~' && isVariable( equation[ i + 1 ] ) ) )
		{
			pfEquation += equation[i];
			
			if( equation[i] == '~' )
			{
				i++;
				pfEquation += equation[i];
			}

			pfEquation += ' ';
			
			// Check for implied multiplication
			if( checkImplyMultiply( i + 1 ) )
			{
				i--;
			}
		}
		/* ----- Numbers ----- */
		else if( isNumStart( equation[i] ) )
		{
			// Used to prevent multiple periods in a number
			bool hasPeriod = false;
			
			// Check if the number is negative (~ means negative)
			if( equation[i] == '~' )
			{
				pfEquation += '~';
				i++;
			}

			// Loop through the infix equation until the end of the number and
			// store it in pfEquation.
			while(
				equation.length() > i && (
					( !hasPeriod && equation[i] == '.' )
					|| isNum( equation[i] )
				)
			)
			{
//				cout << "Checking " << equation[i] << endl;
//				cout << "is it a number? " << isNum( equation[i] ) << endl;
				if( !hasPeriod && equation[i] == '.' )
				{
					hasPeriod = true;
				}

				pfEquation += equation[i];
				i++;
			}
			i--;

			// Adds implied coefficient of one in case of ~(...)
			if( equation[i] == '~' )
			{
				pfEquation += '1';
			}

			pfEquation += ' ';

			// Check for implied multiplication
			if( checkImplyMultiply( i + 1 ) )
			{
				i--;
			}
		}
		/* ----- Operators ----- */
		else if( isOperator( equation[i] ) )
		{
			// If any operators on the stack have a higher precedence than the
			// current operator, place them in the postfix equation.
//			cout << "[infToPf::Operators] Found an op: " << equation[i] << endl;
			while( !opStack.empty()			 // Convert char to string
				&& hasPrecedence( opStack.top(), string("") + equation[i] ) )
			{
				pfEquation += opStack.top();
				pfEquation += ' ';
				opStack.pop();
			}

			opStack.push( string("") + equation[i] );
//			cout << "pfEquation: " << pfEquation << endl;
		}
		/* ----- Functions ----- */
		else if( isLetter( equation[i] ) )
		{
			// Get full function
			int funcStart = i;
			int funcLength = 1;
			i++;
			// MAY CAUSE ISSUES IF VARIABLE IS CHANGED TO A LETTER THAT'S ALSO
			// IN A MATH FUNCTION NAME
			while( i < equation.length() && isLetter( equation[i] )
				&& equation[i] != VARIABLE )
			{
				funcLength++;
				i++;
			}
			i--;

			string function = equation.substr( funcStart, funcLength );

			// Functions have a higher precedence than operators
			if( isFunction( function ) )
			{
				opStack.push( function );
			}
			else
			{
				cout << "[InfEquation::infixToPostFix] Invalid function: "
					 << function << "\n";
				exit(1);
			}
		}
		/* ----- Spaces ----- */
		else if( equation[i] != ' ' )
		{
			cout << "[InfEquation::infixToPostfix] Invalid character: "
				 << equation[i] << "\n";
			exit(1);
		}
	}

	return pfEquation;
}

/**************/
/* PfEquation */
/**************/

// Evaluates a number in a given mathematical function
double PfEquation::evalFunction( string function, double num ) // PRIVATE
{
	if( function == "sqrt" ) 
	{
		return pow( num, 0.5 );
	}
	else if( function == "cos" )
	{
		return cos( num );
	}
	else if( function == "sin" )
	{
		return sin( num );
	}
	else if( function == "tan" )
	{
		return tan( num );
	}
	else
	{
		cout << "[PfEquation::evalFunction] Invalid math function: "
			 << function << "\n";
		exit(1);
	}
}

// Replace all occurences of the variable with a value in the equation and
// return it.
string PfEquation::replaceVar( double varVal )  // PRIVATE
{
	string newEquation;
	
	// Loop through equation
	int i = 0;
	while( i < equation.length() )
	{
		if( isVariable( equation[i] ) )
		{
			string num = to_string( varVal );
			
			// Double negative
			if( i != 0 && equation[i-1] == '~' && varVal < 0 )
			{
				// Remove the '~' that was added last loop
				newEquation.pop_back();
				
				// Trims neg sign off
				num = &num[1];
			}
			// Single negative
			else if( varVal < 0 )
			{
				newEquation += '~';
				
				// Trims neg sign off
				num = &num[1];
			}
			
			// Add num to newEquation
			newEquation += num;
		}
		// Copy anything else over
		else
		{
			newEquation += equation[i];
		}
		
		i++;
	}

	return newEquation;
}

PfEquation::~PfEquation()
{

}

//string PfEquation::getEquation() { return equation; }

// Returns the solution to the PfEquation.
// Potential error: top of stack is checked without checking to see if stack is
// empty or not.
double PfEquation::solve( double varVal, bool &validResult )
{
	stack<double> numStack;

	// Replace the var with a value
	string noVarEq = replaceVar( varVal );

	// Loop through equation
	int i = 0;
	while( i < noVarEq.length() )
	{
		/* ----- Numbers ----- */
		if( isNumStart( noVarEq[i] ) )
		{
			string num;

			// Check for negative nums
			if( noVarEq[i] == '~' )
			{
				num += '-';
			}
			else
			{
				num = noVarEq[i];
			}

			// Get the rest of the number
			i++;
			while( i < noVarEq.length() && isNumStart( noVarEq[i] ) )
			{
				num += noVarEq[i];
				i++;
			}

			// Push the num onto the stack
			numStack.push( stod(num) );
		}
		/* ----- Operators ---- */
		else if( isOperator( noVarEq[i] ) )
		{
			// Get terms
			double num2 = numStack.top();
			numStack.pop();

			double num1 = numStack.top();
			numStack.pop();

			// Evaluate expression and put on stack
			double result = performOperation( noVarEq[i], num1, num2 ); 
//			cout << "[PfEquation::solve()] " << num1 << noVarEq[i] << num2 << "\n";
			numStack.push( result );

			// Abort if the result isn't valid (ex: divide by zero)
			if( !isfinite( result ) )
			{
				validResult = false;
				break;
			}
		}
		/* ----- Functions ----- */
		else if( isLetter( noVarEq[i] ) )
		{
			// Get full function
			int funcStart = i;
			int funcLength = 1;
			i++;
			
			while( i < noVarEq.length() && noVarEq[i] != ' ' )
			{
				funcLength++;
				i++;
			}
			i--;

			string function = noVarEq.substr( funcStart, funcLength );

			// Functions have a higher precedence than operators
			if( isFunction( function ) )
			{
				// Abort if the result isn't valid (ex: divide by zero)
				double eval = evalFunction( function, numStack.top() );
				if( !isfinite( eval ) )
				{
					validResult = false;
					break;
				}
				
				numStack.pop();
				numStack.push( eval );
			}
			else
			{
				cout << "[PfEquation::infixToPostFix] Invalid function: "
					 << function << "\n";
				exit(1);
			}
			
		}
		/* ----- Spaces ----- */
		else if( noVarEq[i] != ' ' )
		{
			cout << "[PfEquation::solve()] Invalid character\n";
			exit(1);
		}

		i++;
	}

	// Return result
//	cout << "[PfEquation::solve()] yVal: " << numStack.top() << endl;
	return numStack.top(); // memory leak or nah?
}

