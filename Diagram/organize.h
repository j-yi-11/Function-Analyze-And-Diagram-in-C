#ifndef ORGANIZE_H_
#define ORGANIZE_H_
#include "list.h"
/*
type: struct function
-----------------------------------------------------------------------
Struct function represents the function which will be shown on screen.
-----------------------------------------------------------------------
struct function{
     double x, y; 
     char name;  
};
-----------------------------------------------------------------------
Variables:
"x""y" records the coordinate of the function graph.
"name" records the name of the function.
*/
struct function{
	char name[40];
	double x, y; 
}func[20010];

/*  Function : HaveShown
 *  - - - - - - - - -
 *  Usage : if(HaveShown(funcname, upper))
         {
           ¡­¡­¡­
         }
 *  This function judges whether the function has been shown on the screen before the upper index in the array to avoid shown the same function repeatedly.
 */
int HaveShown(char *funcname, int upper);

/*  Function : PrintToArray
 *  - - - - - - - - -
 *  Usage : n = PrintToArray(ListNode *phead, int startindex);
 *   This function prints the function related with phead from the start index to the array and return the number of function printed to the array this time.
 */
int PrintToArray(ListNode *phead, int startindex);

/*  Function : FindFuncSubscript
 *  - - - - - - - - -
 *  Usage : int index = FindFuncSubscript(char *funcname);
 *  This function finds the subscript of the function with the funcname in the array named func.
 */
int FindFuncSubscript(char *funcname);

/*  Function :  CountFunctionNumber
 *  - - - - - - - - -
 *  Usage : int count = CountFunctionNumber();
 *  This function counts the number of the functions which links with the ¡°entrance function¡±.
 */
int CountFunctionNumber();

/*  Function : ShowGraph
 *  - - - - - - - - -
 *  Usage : ShowGraph();
variable: number0, num1, num2, num3, num4
-----------------------------------------------------------
number0, num1, num2, num3, num4 represent the number of functions that are shown on the screen repectively for each "layer".
number0 is the entrance function, that's number0 = 1;
num1 is the number of the functions on the first layer; 
num2 is the number of the functions on the second layer;
num3 is the number of the functions on the third layer;
num4 is the number of the functions on the fourth layer but it's not usually shown on the screen;
-----------------------------------------------------------
 *  This function shows the calling graph of functions. This function follows the idea that we want to draw a calling graph, we should first copy the data from the linked list to the array because it¡¯s convenient for us to operate. Meanwhile, we can get num1, num2¡­ for the functions we should draw in each layer repectively. Secondly, we ought to show the function names from func[0] on screen and avoid showing them repeatedly. Finally, we should draw the links between the functions on screen from func[0] to complete the task.
 */
void ShowGraph();

/*  Function : NoneFunc
 *  - - - - - - - - -
 *  Usage : NoneFunc();
 *  This function shows the function which the user input can not be found in the current file.
 */
void NoneFunc();
#endif
