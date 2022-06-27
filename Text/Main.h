#ifndef MAIN_H
#define MAIN_H
/* Function: Display()
* - - - - - - - - -
* This function is using to redraw the new graph after clearing the screen is the 
* entry point to achieve the target function. Refresh pages with globalstate, control 
* the execution of different functions
*/
void Display(); 

/* Function: EnterInformation()
* - - - - - - - - -
* When you select the button to put texts, this function is used to enter the file path
* When you choose to generate a graph call relationship, enter both file path and the function name
*/
void EnterInformation();
/* Function: DrawMenu()
* - - - - - - - - -
* This function is using to draw the table of contents and button sections of the 
* interface 
*/
void DrawMenu();

/* Function:Help()
* - - - - - - - - -
* This function can be used to show the contents of users¡¯?guide
*/
void Help();

/* Function: ChangeButtonColor()
* - - - - - - - - -
* This function is use to change buttons¡¯?color by invoking setButtonColors in 
* libgraphics 
*/
void ChangeButtonColor();

/* Function:ChangeBackColor();
* - - - - - - - - -
* This function is use to change the color of page background
*/
void ChangeBackColor();

/* Function:OverSize();
* - - - - - - - - -
* This function can determine if the number of functions under the file path 
* exceeds the upper paint limit, which means there are more than 14 functions.
*/
void Oversize();

/* Function:None();
* - - - - - - - - -
* This function can determine whether it is a .c files in this directory 
* through the analysize() function
*/
void None();

/* Function:GetDataFromClip(int i);
* - - - - - - - - -
*  When i = 1, copy the contents of the clipboard under foldpath[100];
*  When i = 2, copy the contents of the clipboard under entrancefunc[100];
*/
void GetDataFromClip(int i);

/* Function:SetDataToClip(void);
* - - - - - - - - -
* Save the content to the clipboard
*/
void SetDataToClip(void);
void DrawCenteredCircle(double x, double y, double r);
#endif
