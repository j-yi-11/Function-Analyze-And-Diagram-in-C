#include <stdio.h>
#include <windows.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "boolean.h"
#include "exception.h"
#include "extgraph.h"
#include "gcalloc.h"
#include "genlib.h"
#include "graphics.h"
#include "linkedlist.h"
#include "strlib.h"
#include "imgui.h"
#include "analyze.h"
#include "PutText.h"
#include "organize.h"
#include "Main.h"
double winwidth, winheight;/*the width and height of graphical interface*/
int page = 0;/*page on behalf of the total pages*/
static double RanNumber[3];/*store the random numbers which control the color of background（TODO:之后选定几个背景颜色后可以更改）*/ 
char foldpath[200];/*holds the path to the file entered by the user*/ 
char entrancefunc[200];
int isdrawing = 0; 
extern int tot, fontindex;
extern struct function func[20010];
GlobalState globalstate, prevstate; 

void CharEventProcess(char ch){
	uiGetChar(ch);  
    Display(); 
}

void KeyboardEventProcess(int key, int event){
	uiGetKeyboard(key, event); 
    	Display(); 
}

void MouseEventProcess(int x, int y, int button, int event){
	uiGetMouse(x, y, button, event); 
		Display();
}
   
void SetDataToClip(void)    
{
    // 打开剪贴板   
    if (!OpenClipboard(NULL)|| !EmptyClipboard()){   
        return;   
    }       
    HGLOBAL hMen;   
    TCHAR strText[256] = "我被拷贝到剪切板！/n";      
    // 分配全局内存    
    hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(strText)+1)*sizeof(TCHAR)));      
    if (!hMen){   
        // 关闭剪切板    
        CloseClipboard();   
        return;         
    }     
    // 把数据拷贝考全局内存中   
    // 锁住内存区                                
    LPSTR lpStr = (LPSTR)GlobalLock(hMen);       
    // 内存复制   
    memcpy(lpStr, strText, ((strlen(strText))*sizeof(TCHAR)));    
    // 字符结束符                                  
    lpStr[strlen(strText)] = (TCHAR)0;   
    // 释放锁                                                                  
    GlobalUnlock(hMen);
    // 把内存中的数据放到剪切板上   
    SetClipboardData(CF_TEXT, hMen);   
    CloseClipboard();   
    return;   
}   
void GetDataFromClip(int i)   
{                                   
    // 判断数据格式    
    if (!IsClipboardFormatAvailable(CF_TEXT) )   
    {    
        return;   
    }   
																		  
    if (!OpenClipboard(NULL))    
    {   
        return;         
    }   
    // 从剪切板获取数据    
    HGLOBAL hMem = GetClipboardData(CF_TEXT);   
    if (hMem)   
    {    
        //  获取字符串    
        LPSTR lpStr = (LPSTR)GlobalLock(hMem);     
        if (lpStr)   
        {   
            if(i == 1){
            	strcpy(foldpath,lpStr);
            }   
            else{
            	strcpy(entrancefunc,lpStr);
            }
            // 释放内存锁    
            GlobalUnlock(hMem);    
        }   
    }    
    // 关闭剪切板    
    CloseClipboard();    
}   

void InitColors(){
	RanNumber[0] = 1;
	RanNumber[1] = 1;
	RanNumber[2] = 1;
	DefineColor("Any", RanNumber[0], RanNumber[1], RanNumber[2]);
	DefineColor("Light Blue", .69, .84, .9);
	DefineColor("silver", .92, .92, .92);
	DefineColor("Silver", .443, .443, .443);
	DefineColor("Dark Blue", .18, .21, .33);
	DefineColor("Dark Cyan", .12, .38, .455);
	DefineColor("Orange", 1, .8, .32);
}

void Main()  {
	SetWindowTitle("Global Function Invoke Profiling");
    InitGraphics();
    InitColors();
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
	registerCharEvent(CharEventProcess);  
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	memset(foldpath, '\0', sizeof(foldpath));
	memset(entrancefunc, '\0', sizeof(entrancefunc));
	Display();
}

void Display(){
	DisplayClear();
	fontindex = 1;
	if(globalstate == HomePage){
		memset(foldpath, '\0', sizeof(foldpath));
    	memset(entrancefunc, '\0', sizeof(entrancefunc));
	} 
	if(globalstate == HomePage || globalstate == TextEnterPath || 
	   globalstate == OverSize || globalstate == DiagramEnterPath)
	{    
		DrawMenu();/*draw the menu and button*/ 
	} 
	if(globalstate == TextEnterPath || globalstate == OverSize || 
	   globalstate == DiagramEnterPath)
	{
	   EnterInformation();/*enter the file path*/ 
	}
	switch(globalstate)
	{
		case OneFunctionDiagram:
			isdrawing = 1;
			ShowGraph();
			break;
		case TextFunction:
			page = tot / 6 + 1;/*output as text*/ 
			isdrawing = 0;
			TextPlay();
			break;
		case NoFunction:
			None();/*there isn't any .c files in this folder*/
			break;
		case ShowHelp:
		    Help();/*write down the instruction*/
			break;	
		case OverSize:
			Oversize();/*when tot>14*/
			break;
		case FunctionNotFound:
			NoneFunc();
			break;
		case ReadFromFile:
			ReadResult();
			break; 
		default:
			break;
	}
}
void DrawCenteredCircle(double x, double y, double r)
{
	SetPenSize(6);
	SetPenColor("Light Blue");
    MovePen(x + r, y);
    DrawArc(r, 0.0, 360.0);
    SetPenSize(2);
}
void DrawMenu(){//draw the menu and bu
	SetPenSize(2);
	DefineColor("Any", RanNumber[0], RanNumber[1], RanNumber[2]);
	SetPenColor("Any");
	drawRectangle(winwidth / 5.2, winheight / 1000, winwidth, winheight / 1.04, 1);
	SetPenColor("Dark Blue");
	drawRectangle(winwidth / 400, winheight / 500, winwidth / 5.2, winheight / 1.036, 0);
	SetPenColor("Black");
	SetPenSize(3);
	drawLabel(winwidth / 15, winheight / 1.08, "工具栏");
	SetPenSize(2);
	MovePen(winwidth / 400, winheight / 1.12);
	DrawLine(winwidth / 5.2, 0);
	static char * menuListFile[] = { "   > 设置",  
									 "读取结果文件  | Ctrl-O", 
		  							 "文字输出调用关系  | Ctrl-P",
									 "函数调用关系图   | Ctrl-N",
									 "退出  | Ctrl-E" };
	static char * menuListHelp[] = {"     > 关于",
									"帮助  | Ctrl-M"};
	static char * menuListSight[] = {"     > 视图",
									 "改变按键颜色" ,
									 "改变背景颜色" };
	static char *selectedLabel = NULL;               
	double fH = GetFontHeight();                                     
	double x = winwidth / 5.2; 
	double y = winheight;
	double h = fH * 1.4; 
	double H = winheight / 10;
	int m;
	double w = TextStringWidth(menuListHelp[1]) * 2.4;
	double wlist = TextStringWidth(menuListFile[1]) * 1.5;
	double r = 0.04; /*圆半径*/
	SetPenColor("Light Blue");
	drawRectangle(winwidth / 400, winheight / 1.03, winwidth / 5.2, winheight / 20.9, 1);
	int selection;
	drawMenuBar(0, y - h, winwidth, h);
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, 
	menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if(selection == 1){
		globalstate = ReadFromFile;
		Display();
	} 
	if(selection == 2){  
		globalstate = TextEnterPath;
		Display();
	}//enter path before drawing global function diagram or outputing in text
	if(selection == 3){
		globalstate = DiagramEnterPath;
		Display();
	}//draw diagram only about this function
	if(selection == 4){
		exit(-1);
	}//exit 
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, 
						 menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if(selection == 1){
		globalstate = ShowHelp;
		Display();
	}//help
	selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListSight, 
						 sizeof(menuListSight) / sizeof(menuListSight[0]));
	if(selection == 1){
		ChangeButtonColor();
		Display();
	}//change button color
	if(selection == 2){
		ChangeBackColor();
		Display();
	}//change background color
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - 5*H, 
			  winwidth / 7.3, winheight / 20, "改变按键颜色")){	
	     ChangeButtonColor();
	     Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 4.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - 4*H, 
			  winwidth / 7.3, winheight / 20, "改变背景颜色")){
		ChangeBackColor();
		Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 3.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - 3*H, 
			  winwidth / 7.3, winheight / 20, "读取结果文件")){
		globalstate = ReadFromFile;
		Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 2.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - 2*H,
	 		  winwidth / 7.3, winheight / 20, "文字输出调用关系")){
		globalstate = TextEnterPath;
		Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 1.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - H, 
			  winwidth / 7.3, winheight / 20, "函数调用关系图")){
		globalstate = DiagramEnterPath;
		Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 0.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 1.1 - H * 6, 
			  winwidth / 8, winheight / 20, "帮助")){
		globalstate = ShowHelp;
		Display();
	}
	DrawCenteredCircle(winwidth / 80, winheight / 1.1 - 5.75 * H, r);
	if(button(GenUIID(0), winwidth / 32, winheight / 5, 
			  winwidth / 8, winheight / 20, "退出")){
		exit(0);
	}
	DrawCenteredCircle(winwidth / 80, winheight / 4.5, r);
}

void Help(){
	DrawMenu();
	char *str[7]={"欢迎使用全局函数调用分析工具",
				  "如果你希望保存本次函数分析结果，请点击按钮保存信息到文件",
				  "如果你希望看见函数调用的文字关系，请点击文字输出调用关系",
				  "输入要分析的函数所在的文件的路径,例如 D:\\user\\Documents\\code点击确定", 
				  "如果你希望分析函数调用关系图，则点击函数调用关系图,并输入文件和函数名",
				  "输入结束后，即可生成全局函数调用关系图或函数分析图",
				  "你也可以改变当前按钮的颜色以及背景颜色"};
	int i;
	double x = winwidth / 3.7, y = winheight / 1.2, h = winheight / 15;
	SetPenColor("Brown");
	SetPenSize(2);
	drawRectangle(winwidth / 4, winheight / 6, winwidth / 1.4, winheight / 1.4, 0);
	DrawCenteredCircle(winwidth / 3.5, winheight / 1.2, 0.2);
		SetPenColor("Black");
	SetPenSize(20);
	SetStyle(1);
	MovePen(winwidth / 3, y);
	DrawTextString(str[0]);
	y -= 2 * h;
	SetStyle(0);
	SetPenSize(12);
	for(i = 1; i < 7; i++){
		MovePen(x, y);
	    DrawTextString(str[i]);
	    y -= h;
	}
	SetPenSize(2);
	SetPenColor("Dark Cyan");
	MovePen(x,winheight / 1.41);
	DrawLine(-1.2, -0.3); 
	MovePen(x,winheight / 1.6);
	DrawLine(-1.1, 1.4); 
	MovePen(x, winheight / 1.6-2 * h);
	DrawLine(-1, 1.45); 
	if(button(GenUIID(0), winwidth / 1.3, winheight / 5, 
	winwidth / 10, winheight / 20, "退出帮助")){
		globalstate = HomePage;
	}
}

void EnterInformation(){
	char ch[100];
	double r = 0.08;
	SetPenColor("Brown");
	drawRectangle(winwidth / 4, winheight / 8.5, winwidth / 1.4, winheight / 1.4,0);
	SetPenColor("Dark Cyan");
	SetPenSize(4);
	DrawCenteredCircle(winwidth / 1.8, winheight / 1.28, r);
	SetPenSize(2);
	SetPenColor("Brown");
	drawLabel(winwidth / 1.7, winheight / 1.3, "工作栏");
	MovePen(winwidth / 4, winheight / 1.35);
	DrawLine(winwidth / 1.4, 0);
	SetPenColor("Dark Cyan");
	if(globalstate == TextEnterPath){  
		prevstate = globalstate;
    	drawLabel(winwidth / 2 - TextStringWidth("输入文件路径") * 1.4, winheight / 1.6, "输入文件路径");
    if(button(GenUIID(0), winwidth / 1.5, winheight / 4,
		   winwidth / 6, winheight / 20, "粘贴文件路径")){
    	GetDataFromClip(1); 
    }
	    textbox(GenUIID(0), winwidth / 2.5, winheight / 2.2, 
    		winwidth / 2, winheight / 8, foldpath, sizeof(foldpath));//input folderpath 
		if(button(GenUIID(0), winwidth / 2.2, winheight / 4,
		   winwidth / 10, winheight / 20, "确定"))
		{
			*(foldpath + strlen(foldpath)) = '\0';//recieve a right file path
    		Analyze(foldpath);//analyze it
    		if(globalstate != NoFunction)
    			globalstate = TextFunction;
    		Display();
		}
	}
	else if(globalstate == DiagramEnterPath){
		prevstate = globalstate;  
    	SetPenColor("Silver"); 
		drawLabel(winwidth / 1.9 - TextStringWidth("输入文件路径") * 1.4, winheight / 1.6, "输入文件路径");
		if(button(GenUIID(0), winwidth / 2.5, winheight / 7.2,
		   winwidth / 6, winheight / 20, "粘贴文件路径")){
    	GetDataFromClip(1); 
    }
		textbox(GenUIID(0), winwidth / 2.6, winheight / 2.05, 
				winwidth / 2, winheight / 9, foldpath, sizeof(foldpath));//input folderpath 
	if(button(GenUIID(0), winwidth / 1.4, winheight / 7.2,
		   winwidth / 8, winheight / 20, "粘贴函数名")){
    	GetDataFromClip(2); 
    }	  
        SetPenColor("Silver");
		drawLabel(winwidth / 2 - TextStringWidth("输入函数名") * 1.4, winheight / 2.3, "输入函数名");
		textbox(GenUIID(0), winwidth / 2.6, winheight / 3.3, 
			 	winwidth / 2, winheight / 9, entrancefunc, sizeof(entrancefunc)); 
		if(button(GenUIID(0), winwidth / 1.4, winheight / 5,
				  winwidth / 10, winheight / 20, "返回")){
     		globalstate = HomePage;
     		Display();
    	}//initialize the path when returning     
		if(button(GenUIID(0), winwidth / 2.3, winheight / 5,
			winwidth / 10, winheight / 20, "确定"))
		{
	    	*(foldpath + strlen(foldpath)) = '\0';//recieve a right file path
	    	Analyze(foldpath);
	    	if(globalstate != NoFunction){
	        	*(entrancefunc + strlen(entrancefunc)) = '\0';
	        	strcpy(func[0].name, entrancefunc);
	        	if(CountFunctionNumber() > 14){
	        		globalstate = OverSize;
	        		Display();
	    		}	    	
    			else{
	    			globalstate = OneFunctionDiagram;
	    			Display();
	    		}	
	    	}
		}    	
	}
}

void ChangeButtonColor(){//change button
		char *c[] = {"Black" , "White" , "Dark Gray" , "Light Gray" , "Cyan",
				     "Brown" , "Green" , "Blue" };
		int N = sizeof(c) / sizeof(c[0]);
		static int times; 
		times++;
		setButtonColors(c[times % N], c[(times + 1) % N], c[(times + 2) % N], c[(times + 3) % N], 1);
}

void ChangeBackColor(){//change background
	static int m = 0;
	int i;
	double Ran[3][3]={1, 1, 1, .6, .73, .62, .91, .87, .8};
	m++;
	if(m >= 3){
		m = 0;
	}
	for(i = 0; i < 3; i++){
		RanNumber[i] = Ran[m][i];
	}
}

void Oversize(){//when there are more than 11 functions to be analyzed in the file, the complete call diagram cannot be d
    SetPenColor("Orange");
	drawRectangle(winwidth / 2.5, winheight / 2, winwidth / 20, winheight / 7, 1);
	SetPenColor("Dark Cyan");
	drawLabel(winwidth / 1.3 - TextStringWidth("该文件调用函数过多，无法显示完整函数图"), winheight / 1.9, 
			  "该文件调用函数过多，无法显示完整函数图");
	if(button(GenUIID(0),winwidth / 3.2, winheight / 6,
			  winwidth / 9, winheight / 20, "重新输入")){
	    globalstate = DiagramEnterPath;
	    memset(foldpath, '\0', sizeof(foldpath));
    	memset(entrancefunc, '\0', sizeof(entrancefunc));
	    Display();
    }
    setButtonColors("White", "Dark Gray", "Light Gray", "Light Blue", 1);
	if(button(GenUIID(0), winwidth / 1.4, winheight / 6, 
			  winwidth / 12, winheight / 20, "返回")){
	    globalstate = HomePage;
	    Display();
    }
	if(button(GenUIID(0),winwidth / 3.2, winheight / 4,
			  winwidth / 6, winheight / 20, "以文字形式展现")){
	    globalstate = TextFunction;
	    Display();
    }
    if(button(GenUIID(0),winwidth / 1.5, winheight / 4,
			  winwidth / 6, winheight / 20, "显示部分调用关系")){
	    *(entrancefunc + strlen(entrancefunc)) = '\0';
	    sprintf(func[0].name, "%s", entrancefunc);
	    globalstate = OneFunctionDiagram;
	    ShowGraph();
    } 
}
void None(){
//there are no .c files in the current directory 
	SetPenColor("Red");
	drawRectangle(winwidth / 4.5, winheight / 1.6, winwidth / 10, winheight / 5, 1);
	if(button(GenUIID(0), winwidth / 1.5, winheight / 4, 
			  winwidth / 12, winheight / 20, "返回")){
		globalstate = HomePage;
	}
	if(button(GenUIID(0), winwidth / 1.9, winheight / 4, 
			  winwidth / 12, winheight / 20, "帮助")){
		globalstate = ShowHelp;
	}
	SetPenSize(10);
	SetPenColor("Red");
	MovePen(winwidth / 3, winheight / 1.4);
	DrawTextString("ERROR!");
	SetPenColor("Black");
	MovePen(winwidth / 3, winheight / 1.5);
	DrawTextString("当前目录下没有c文件");
	DrawCenteredCircle(winwidth / 4, winheight / 1.9, 0.2);
	SetPenColor("Dark Blue");
	drawRectangle(winwidth / 3.5, winheight / 2.4, winwidth / 2, winheight / 5, 0);
	SetPenColor("Black");
	MovePen(winwidth / 3, winheight / 1.9);
	DrawTextString("请您重新检查文件路径");
	MovePen(winwidth / 3, winheight / 2.1);
	DrawTextString("确保该路径下至少有一个可以编译的.c文件");
	if(button(GenUIID(0), winwidth / 3, winheight / 4, 
			  winwidth / 6, winheight / 20, "重新输入路径")){
		globalstate = prevstate;
		*foldpath = '\0';
		*entrancefunc = '\0'; 
		Display();
	}
}
void NoneFunc()
{
 //this function cannot be found in current file
    SetPenColor("Red");
    drawRectangle(winwidth / 4.5, winheight / 1.6, winwidth / 10, winheight / 5, 1);
    if(button(GenUIID(0), winwidth / 1.5, winheight / 4, 
         winwidth / 12, winheight / 20, "返回")){
          globalstate = HomePage;
     }
     if(button(GenUIID(0), winwidth / 1.9, winheight / 4, 
        winwidth / 12, winheight / 20, "帮助")){
      globalstate = ShowHelp;
     }
     SetPenSize(10);
    SetPenColor("Red");
    MovePen(winwidth / 3, winheight / 1.4);
    DrawTextString("ERROR!");
    SetPenColor("Black");
    MovePen(winwidth / 3, winheight / 1.5);
    DrawTextString("待查询函数未保存在当前文件文件路径");
    DrawCenteredCircle(winwidth / 4, winheight / 1.9, 0.2);
    SetPenColor("Dark Blue");
    drawRectangle(winwidth / 3.5, winheight / 2.4, winwidth / 1.8, winheight / 5, 0);
    SetPenColor("Black");
    MovePen(winwidth / 3, winheight / 1.9);
    DrawTextString("请您重新检查文件路径与函数名，确保该函数保存在当前文件下");
    MovePen(winwidth / 3, winheight / 2.1);
    DrawTextString("您不需要输入函数类型或参数表，直接输入正确的函数名即可查询");
    if(button(GenUIID(0), winwidth / 3, winheight / 4, 
     winwidth / 6, winheight / 20, "重新输入路径")){
    globalstate = DiagramEnterPath;
     *foldpath = '\0';
     *entrancefunc = '\0'; 
    }
}
