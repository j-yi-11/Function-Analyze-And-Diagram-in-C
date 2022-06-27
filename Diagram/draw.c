#include "draw.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include <math.h>
#include <string.h>
int fontindex = 0;
void ChangeFontStyle()
{
	const int fontnum = 3;
	SetStyle(fontindex);
	fontindex = (fontindex+1)%fontnum;
}
void DrawTriangle(double beta)
{
	double alpha = beta + 150.0 / 180.0 * pi;
	DrawLine(arrowsidelength * cos(alpha), arrowsidelength * sin(alpha));
	alpha += 120.0 / 180.0 * pi;
	DrawLine(arrowsidelength * cos(alpha), arrowsidelength * sin(alpha));
	alpha += 120.0 / 180.0 * pi;
	DrawLine(arrowsidelength * cos(alpha), arrowsidelength * sin(alpha));
	alpha += 120.0 / 180.0 * pi;
}
void DrawArrow(double Start_X, double Start_Y, double End_X, double End_Y, 
               double StartHeight, double StartWidth,
      		   double EndHeight, double EndWidth)
{
	  const double eps = 1e-6;
	  SetPenColor("Red"); 
	  double x, y;
	  if(fabs(Start_Y - End_Y) < eps)
	  {
	    if(Start_X < End_X)  /*Rightwards arc arrow*/ 
	    {
		       MovePen(x = End_X, y = End_Y + EndHeight);
		       double theta = atan(StartHeight / ((End_X - Start_X - StartWidth) / 2)) / pi * 180;
		       double r = sqrt(pow(StartHeight, 2) + pow((End_X - Start_X - StartWidth) / 2, 2));
		       DrawArc(r, theta, 180 - 2 * theta);
		       MovePen(x = End_X, y = End_Y + EndHeight);
		       StartFilledRegion(isfilled);
		       DrawTriangle(atan2(End_Y - Start_Y, End_X - Start_X));
		       EndFilledRegion(); 
	 		   return;
	    }
		   else{         /*Leftwards arc arrow*/ 
			    MovePen(x = Start_X, y = Start_Y + StartHeight);
			    double theta = atan(StartHeight / ((Start_X - End_X - EndWidth) / 2)) / pi * 180;
			    double r = sqrt(pow(StartHeight, 2) + pow((Start_X - End_X - EndWidth) / 2, 2));
			    DrawArc(r, theta, 180 - 2 * theta);  
			    StartFilledRegion(isfilled); 
			    DrawTriangle(atan2(End_Y - Start_Y, End_X - Start_X));
			    EndFilledRegion(); 
			    return;         
		   }
	 }
	 else if(Start_Y > End_Y)  
	 {
		  StartFilledRegion(isfilled);
		  MovePen(x = Start_X + StartWidth / 2, y = Start_Y);
		  DrawLine(End_X + EndWidth / 2 - x, End_Y + EndHeight - Start_Y);
		  DrawTriangle(atan2(End_Y - Start_Y, End_X - Start_X));
		  EndFilledRegion();
		  return;
	 }
	 else if(Start_Y < End_Y)  
	 {
		 StartFilledRegion(isfilled);
		 MovePen(x = Start_X + StartWidth / 2, y = Start_Y + StartHeight);
		 DrawLine(End_X + EndWidth / 2 - x, End_Y - Start_Y - StartHeight);  
		 DrawTriangle(atan2(End_Y - Start_Y, End_X - Start_X));   
	     EndFilledRegion();
	     return;
	 }
}
void DrawCircle(double Start_X, double Start_Y, double width, double height) 
{
     SetPenColor("Red");
 	 double r = 0.5 * sqrt(pow(height, 2) + pow(width / 2,2));
     double theta = atan(height / (width / 2)) / pi * 180;
     MovePen(Start_X + width / 2, Start_Y + height); 
 	 DrawArc(r, theta, 360 - 2 * theta);
	 StartFilledRegion(isfilled); 
	 DrawLine(arrowsidelength / 2, -arrowsidelength / 2 * sqrt(3));
	 DrawLine(-arrowsidelength, 0); 
	 DrawLine(arrowsidelength / 2, arrowsidelength / 2 * sqrt(3)); 
	 EndFilledRegion();
}
void ShowFuncName(char *name, double x, double y)
{
	 SetPenColor("Red");
	 char functionName[800];
	 memset(functionName, '\0', sizeof(functionName)); 
	 strcpy(functionName, name);
	 const int maxlength = 12;
	 if(strlen(functionName) <= maxlength){
	 	strcat(functionName, "   ");
	 }	  
	 else{
	    functionName[0] = functionName[1] = functionName[2] = '.';
	 	strcat(functionName, "   ");  
	 }
	 ChangeFontStyle();
	 textbox(GenUIID(0), x, y, TextStringWidth(functionName), 
	         TextHeight, functionName, sizeof(functionName));         
}
