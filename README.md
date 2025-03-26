# Simple Graphing Calculator
A simple graphing calculator that prints out the resulting graph as ASCII characters.

# Quick compile (Linux, BASH)
Download files and run `make` command. Output will be in `calulator.out`. May require resizing your terminal window.

# Features
- Basic arithmetic (+, -, *, /).
- Exponents (ex: 2^x).
- Simple trig functions (sine, cosine, and tangent).
- Colored lines (Linux only).
- Up to 10 lines on the graph at once.
- Zooming in and out with commands.
- Tick marks and a key to show scale.

# Limitations
- Limited input format checking.
- Only accepts equations that are a function of x.

# How it works
### Infix to postfix conversion
When the user inputs the `plot` command, the program will prompt the user for input, giving them the starting point `y = `. The user input is then converted from infix notation to postfix notation.

- Infix notation: `5 + 4`
- Postfix notation: `5 4 +`

This conversion is done because it is faster and easier to parse a postfix equation than an infix equation. After the convserion, the postfix version will be evaluated many times, which will be explained in the next section.

### Graph structure
The graph where the line will be plotted is stored as a 2D array in the Graph class. The Graph class has a member function for plotting a postfix equation: `graphPfEquation( PfEquation equation)`. Based on the current graph scale and width (in characters), the function calculates the x value at each character. Then, it plugs the x value into the postfix equation, and evaluates the expression to get the y value. 
