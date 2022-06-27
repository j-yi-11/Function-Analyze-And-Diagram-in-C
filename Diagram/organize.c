#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "organize.h"
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "draw.h"
#include "treap.h"
#include "list.h"
#include "analyze.h" 
#include "PutText.h"
#include "Main.h"
int count = 1;
extern int fontindex;
extern int isdrawing;
extern char foldpath[100];/*holds the path to the file entered by the user*/ 
extern char entrancefunc[100];
extern struct value Map[20010];
extern List *call[20010];
extern double winwidth, winheight;  
GlobalState globalstate; 
int number0 = 1, num1 = 0, num2 = 0, num3 = 0, num4 = 0;
int PrintToArray(ListNode *phead, int startindex);
int HaveShown(char* funcname, int upper)
{
	int i;
	for(i = 0 ; i < upper ; i++){
		if (strcmp(funcname, func[i].name) == 0){
			return 1;
		}
		else;
	}
	return 0;
}
int PrintToArray(ListNode *phead, int startindex)
{
	ListNode *j = NULL;
	int i = startindex;
    for(j = phead; j != NULL; j = j->next){
    	Call *a = (Call *)(j->data);
    	if(!HaveShown(Map[a->id].funcname, count))
    		strcpy(func[++i].name, Map[a->id].funcname);
    }	
    return i - startindex;
}
int FindFuncSubscript(char *funcname)
{
	int i;
	for(i = 0; i < count - num4 ; i++)
	{
		if(strcmp(funcname, func[i].name) == 0){
			return i;
		}
		else;
	}
	return -1;
}
int CountFunctionNumber(){
	int up = 0, index = 0;
	int number0 = 1, num1 = 0, num2 = 0, num3 = 0, num4 = 0;
	index = FindFuncByName(func[0].name);
	if(index == -1){  
		globalstate = FunctionNotFound;
		isdrawing = 0;
		Display();
		return -1;
	}
    int i = 0;
    count = 1 + (num1 = PrintToArray(call[index]->head, 0));  
    if(count>14) return count;
    for(i = 1, up = num1; i <= up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num2 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2;
    }
    if(count>14)  return count;
    for(i = num1 + 1, up = count; i < up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num3 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2 + num3;
    }
    if(count>14)  return count;    
    for(i = num1 + num2 + 1, up = count; i < up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num4 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2 + num3 + num4;
    }
    return count ;  
}
void ShowGraph(){
	DisplayClear();
	int up = 0, index = 0;
	count = 1; 
	double mywinwidth = winwidth /6 * 5;
	int i = 0;
	index = FindFuncByName(func[0].name);
	if(index == -1){  
		globalstate = FunctionNotFound;
		isdrawing = 0;
		Display();
		return;
	}
	number0 = 1, num1 = 0, num2 = 0, num3 = 0, num4 = 0;
    count = 1 + (num1 = PrintToArray(call[index]->head, 0));  
    for(i = 1, up = num1; i <= up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num2 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2;
    }
    for(i = num1 + 1, up = count; i < up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num3 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2 + num3;
    }
    for(i = num1 + num2 + 1, up = count; i < up; i++)
    {
    	index = FindFuncByName(func[i].name);
    	num4 += PrintToArray(call[index]->head, count - 1);
    	count = 1 + num1 + num2 + num3 + num4;
    }    
    /*finish inputing*/    
    for(i = 0; i < number0; i++)
	{
		if(!HaveShown(func[i].name, i)){
			ShowFuncName(func[i].name, func[i].x = mywinwidth / (number0 + 1) * (i + 1), 
						 func[i].y = winheight / 4 * 4 - 2 * TextHeight);
		}
		else{
		}
	}	
	for(i = 0; i < num1; i++)
	{
		if(!HaveShown(func[number0 + i].name, number0 + i)){
			ShowFuncName(func[number0 + i].name, func[number0 + i].x = mywinwidth / (num1 + 1) * (i + 1), 
						 func[number0 + i].y = winheight / 4 * 3 + i % 3 * TextHeight);
		}
		else{
		}
	}
	for (i = 0; i < num2; i++)
	{
		if(!HaveShown(func[number0 + num1 + i].name, number0 + num1 + i)){
			ShowFuncName(func[number0 + num1 + i].name, func[number0 + num1 + i].x = mywinwidth/(num2 + 1) * (i + 1),
						 func[number0 + num1 + i].y = winheight / 4 * 2 + i % 3 * TextHeight);
		}
		else{
		}
	}
	for (i = 0; i < num3; i++)
	{
		if(!HaveShown(func[number0 + num1 + num2 + i].name, number0 + num1 + num2 + i)){
			ShowFuncName(func[number0 + num1 + num2 + i].name, func[number0 + num1 + num2 + i].x = mywinwidth / (num3 + 1) * (i + 1), 
						 func[number0+num1+num2+i].y = winheight / 4 * 1 + i % 3 * TextHeight);
		}
	}
	/*finish showing*/	
	
	for(i = 0; i < count - num4 ; i++)
	{
		int startid = FindFuncByName(func[i].name);
		ListNode *j = NULL;
        for(j = call[startid]->head; j != NULL; j = j->next){
	    	Call *a = (Call *)(j->data);    
	    	if(strcmp(func[i].name, Map[a->id].funcname) == 0)
	    	{
	    		DrawCircle(func[i].x, func[i].y, 
						   TextHeight, TextStringWidth(func[i].name));
	    	}else{
	    		int end = FindFuncSubscript(Map[a->id].funcname);
	    		if(end == -1)
					goto mybreak;
				if(func[i].x != 0 && func[i].y != 0)
	    		DrawArrow(func[i].x, func[i].y, func[end].x, func[end].y,
						  TextHeight, TextStringWidth(func[i].name),
						  TextHeight, TextStringWidth(func[end].name));
	    	}
	    	mybreak:
	    		continue;
        }
	}
	/*finishing drawing*/
	if(button(GenUIID(0), winwidth / 1.3, winheight / 5,
		winwidth / 12, winheight / 20, "·µ»Ø")){
    	memset(foldpath, '\0', sizeof(foldpath));
    	memset(entrancefunc, '\0', sizeof(entrancefunc));
     	globalstate = HomePage;
     	isdrawing = 0;
     	Display();
    }
}

