#ifndef DRAW_H_
#define DRAW_H_
#define TextHeight 2*GetFontHeight()
#define arrowsidelength 0.1
#define isfilled 0.99
#define pi acos(-1)
/*  Function : ChangeFontStyle
 *  - - - - - - - - -
 *  Usage : ChangeFontStyle();
 *  This function changes the font style when called.
 */
void ChangeFontStyle();

/*  Function : DrawTriangle
 *  - - - - - - - - -
 *  Usage : DrawTriangle(beta);
 *   This function decides the direction of the arrow when drawing the arrows.
 */
void DrawTriangle(double beta);

/*  Function : DrawArrow
 *  - - - - - - - - -
 *  Usage : DrawArrow(double Start_X, double Start_Y, double End_X, double End_Y, double StartHeight, double StartWidth,double EndHeight, double EndWidth);
 *   This function draws an arrow from the starting point(Start_X,Start_Y) to the ending point(End_X,End_Y). The image of the starting and ending point are rectangles with (StartHeight,  StartWidth) and (EndHeight, EndWidth) respectively to illustrate that one function calls another function.
 */
void DrawArrow(double Start_X, double Start_Y, double End_X, double End_Y, double StartHeight, double StartWidth,double EndHeight, double EndWidth);

/*  Function : DrawCircle
 *  - - - - - - - - -
 *  Usage : DrawCircle(double Start_X, double Start_Y, double width, double height)
 *  This function draws a circle from the midpoint of upper side to the down side.
 */
void DrawCircle(double Start_X, double Start_Y, double width, double height);

/*  Function : ShowFuncName
 *  - - - - - - - - -
 *  Usage :  ShowFuncName(name, x, y)
 *   This function shows the name of one called function in a rectangle according to (x,y).
 */
void ShowFuncName(char *name, double x, double y);
#endif
