#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extgraph.h"
#include "imgui.h"
#include "analyze.h"
#include "list.h"
#include "PutText.h"
#include "Treap.h"
extern double winwidth, winheight;
extern int page;
extern GlobalState globalstate;
extern int tot;
extern char foldpath[100],entrancefunc[100];
static int now = 1, item = 1;
extern List *call[MAXN], *caller[MAXN];
extern struct value Map[MAXN];
static double x, y, xstart, ystart, h;
static TextState textstate = NoChangePage;
void TextPlay(){
    DisplayClear();
	if(textstate == PrevPage || textstate == NextPage){
		TurnPage();
	}//turn to the next page or last page 
	else{
		PutText();
    	PageButton();
	}//output the text and draw the button 
}

void PageButton(){//draw the buttons which allow users to turn pages 
	if(button(GenUIID(0), winwidth / 1.6, winheight / 40,
	   		  winwidth / 12, winheight / 30, "上一页")){
		textstate = PrevPage;
		TextPlay();
	}
	if(button(GenUIID(0), winwidth / 1.4, winheight / 40, 
	   		  winwidth / 12, winheight / 30, "下一页")){
	    textstate = NextPage;
		TextPlay();
	}
	if(button(GenUIID(0), winwidth / 1.2, winheight / 40, 
 	   		  winwidth / 12, winheight / 30, "返回")){
		now = 1;
		textstate = NoChangePage;
		item = 1;
		*foldpath = '\0';
		*entrancefunc = '\0';
		globalstate = HomePage;
	}//initialize each global variable for next input 
}

void PutText(){//output the text 
	x = winwidth / 40;
    y = winheight / 1.05;
    h = winheight / 30;
    int max = 7;
    int issput = 0;
	char str[100], str1[100] = "";
	int count = 1, i;
	struct value val;
    SetPenColor("Brown");
    FILE *fp;
    fp = fopen("result.data", "wb");
	for(i = item; i <= tot; i++){ 
    	xstart = x;
    	ystart = y;
    	val = Map[FindFuncByRank(i)];
    	Changexy();
    	MovePen(x, y);
    	DrawTextString(val.filename);
    	fwrite(val.filename, sizeof(char), strlen(val.filename), fp);
    	fwrite("\n", sizeof(char), 1, fp);
    	y = y - h;
    	Changexy();
    	MovePen(x, y);
    	DrawTextString(val.funcname);
    	fwrite(val.funcname, sizeof(char), strlen(val.funcname), fp);
    	fwrite("\n", sizeof(char), 1, fp);
    	ListNode *j;
    	y = y - h;
    	Changexy();
    	MovePen(x, y);
    	DrawTextString("引用的函数:");//输出储存在call里该函数引用的函数 
    	fwrite("引用的函数:", sizeof(char), sizeof("引用的函数:"), fp);
    	fwrite("\n", sizeof(char), 1, fp);
    	y -= 1.2 * h;
    	for(j = call[val.id]->head; j != NULL; j = j->next){
    	    Call *a = (Call *)(j->data);
    	    issput = issput + 1;
    	    if(strcmp(str1, Map[a->id].funcname) == 0){
    	    	count++;
    	    	continue;
    	    }
    	    strcpy(str1, Map[a->id].funcname);
    	    MovePen(x, y);
            DrawTextString(Map[a->id].funcname);
            fwrite(Map[a->id].funcname, sizeof(char), strlen(Map[a->id].funcname), fp);
            fwrite("\n", sizeof(char), 1, fp);
    	    y -= 1.2 * h;
    	    Changexy();
    	    MovePen(x, y);
    	    itoa(Map[a->id].line, str, 10);
    	    DrawTextString("line:");
    	    fwrite("line:", sizeof(char), strlen("line:"), fp);
    	    fwrite("\n", sizeof(char), 1, fp);
    	    MovePen(x + h * 2, y);
            DrawTextString(str);
            fwrite(str, sizeof(char), strlen(str), fp);
            fwrite("\n", sizeof(char), 1, fp);
            itoa(count, str, 10);
            fwrite(str, sizeof(char), strlen(str), fp);
            fwrite("\n", sizeof(char), 1, fp);
    	    if(count > 1){
    	    	MovePen(x + h * 4, y);
                DrawTextString(str);
                fwrite(str, sizeof(char), strlen(str), fp);
                fwrite("\n", sizeof(char), 1, fp);
                MovePen(x + h * 5, y);
                DrawTextString("times");
                fwrite("times", sizeof(char), strlen("times"), fp);
                fwrite("\n", sizeof(char), 1, fp);
                count=1;
    	    }
    	    if(issput >= max / 2){
    	    	y -= 1.2 * h;
        	    Changexy();
    	    	break;
    	    }
    	    y -= 1.2 * h;
    	    Changexy();
        }
        fwrite("End", sizeof(char), strlen("End"), fp);
        fwrite("\n", sizeof(char), 1, fp);
        y -= 1.2 * h;
        Changexy();
        MovePen(x, y);
        DrawTextString("引用它的函数:");//输出储存在caller里引用该函数的函数 '
        fwrite("引用它的函数:", sizeof(char), strlen("引用它的函数:"), fp);
        fwrite("\n", sizeof(char), 1, fp);
    	y -= 1.5 * h;
    	for(j = caller[val.id]->head; j != NULL; j = j->next){
    	    Call *a = (Call *)(j->data);
    	    issput = issput + 1;
    	    if(strcmp(str1, Map[a->id].funcname) == 0){
    	    	count++;
    	    	continue;
    	    }
    	    strcpy(str1, Map[a->id].funcname);
    	    MovePen(x, y);
            DrawTextString(Map[a->id].funcname);
            fwrite(Map[a->id].funcname, sizeof(char), strlen(Map[a->id].funcname), fp);
            fwrite("\n", sizeof(char), 1, fp);
    	    y -= 1.2 * h;
    	    Changexy();
    	    MovePen(x, y);
    	    itoa(Map[a->id].line, str, 10);
    	    DrawTextString("line:");
    	    fwrite("line:", sizeof(char), strlen("line:"), fp);
    	    fwrite("\n", sizeof(char), 1, fp);
    	    MovePen(x + h * 2, y);
            DrawTextString(str);
            fwrite(str, sizeof(char), strlen(str), fp);
            fwrite("\n", sizeof(char), 1, fp);
            itoa(count, str, 10);
            fwrite(str, sizeof(char), strlen(str), fp);
            fwrite("\n", sizeof(char), 1, fp);
    	    if(count > 1){
    	    	MovePen(x + h * 4, y);
                DrawTextString(str);
                fwrite(str, sizeof(char), strlen(str), fp);
                fwrite("\n", sizeof(char), 1, fp);
                MovePen(x + h * 5, y);
                DrawTextString("times");
                fwrite("times", sizeof(char), strlen("times"), fp);
                fwrite("\n", sizeof(char), 1, fp);
                count=1;
    	    }
    	    if(issput == max)
    	    {
    	    	y -= 1.2 * h;
        	    Changexy();
    	    	issput = 0;
    	    	break;
    	    }
    	    y -= 1.2 * h;
    	    Changexy();
        }
        fwrite("End", sizeof(char), strlen("End"), fp);
        fwrite("\n", sizeof(char), 1, fp);
	    drawRectangle(xstart - 0.5 * h, ystart + h, 10 * h, y - ystart, 0);//分隔不同函数 
        y -= 1.2 * h;
    	if(y < winheight / 10){
			x = x + 11 * h;
        	y = winheight / 1.05;
		}
		if(i % 6 == 0){
			break;//每页输出6个函数 
		}
    }
    Page();//输出页码 
    fclose(fp);
}

void Changexy(){//To determine whether the output is to the last row, if yes then transfer the column 
	if(y < winheight / 10){
		drawRectangle(xstart - 0.5 * h, ystart + h, 10 * h, y - ystart, 0);
		x = x + 11 * h;
        y = winheight / 1.05;
        xstart = x;
        ystart = y;
	}
}

void Page(){//output the total pages and the current page number
	char str[100], p[100];
	itoa(page, p, 10);
	itoa(now, str, 10);
	SetPenColor("Brown");
	MovePen(winwidth / 2.3, winheight / 40);
    DrawTextString("第");
    MovePen(winwidth / 2.1, winheight / 40);
    DrawTextString(str);
    MovePen(winwidth / 2,winheight / 40);
    DrawTextString("页 / ");
    MovePen(winwidth / 1.8, winheight / 40);
    DrawTextString(p);
    MovePen(winwidth / 1.72, winheight / 40);
    DrawTextString("页"); 
}

void TurnPage(){//paging
	if(textstate == NextPage){
		now++;//the current page number plus 1
	}//turn to next page
	else{
		now--;
	}//the current page number minus 1
	if(now > 0 && now <= page){
		item = (now - 1) * 6 + 1;
	}//find the first function in the current page 
	else{
		now = 1, item = 1;
	}//return to the first page
	textstate = NoChangePage;
    TextPlay();
}
static char s[10005];//用于存储读取出来的char 
static int ReadNextString(char *source, int len){
	int i;
	for(i = 0; i < len; i++){
		if(source[i] == '\n') break;
		s[i] = source[i];
	}
	s[i] = 0;
	return i + 1;
}

void ReadResult(){
	SetPenColor("Brown");
	x = winwidth / 40;
    y = winheight / 1.05;
    h = winheight / 30;
	char result[10005], *strp;
	FILE *fp;
	int i, newlen;
    fp = fopen("result.data", "rb");
	int len = fread(result, sizeof(char), 10000, fp);
	for(strp = result; len > 0 && *strp != 0;){ 
    	xstart = x;
    	ystart = y;
    	Changexy();
    	MovePen(x, y);
    	newlen = ReadNextString(strp, len);
    	len -= newlen;
    	strp += newlen;
    	DrawTextString(s);
    	y = y -h;
    	Changexy();
    	MovePen(x, y);
    	newlen = ReadNextString(strp, len);
    	len -= newlen;
    	strp += newlen;
    	DrawTextString(s);
    	y = y -h;
    	Changexy();
    	MovePen(x, y);
    	newlen = ReadNextString(strp, len);
    	len -= newlen;
    	strp += newlen;
    	DrawTextString(s);
    	y -= 1.2 * h;
    	for(;;){
    		printf("%d\n", len);
    	    MovePen(x, y);
            newlen = ReadNextString(strp, len);
            if(strcmp(s, "End") == 0){
            	len -= newlen;
            	strp += newlen;
            	break;
			}
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    	    y -= 1.2 * h;
    	    Changexy();
    	    MovePen(x, y);
    	    newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    	    MovePen(x + h * 2, y);
			newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    		newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		int count = atoi(s); 
    	    if(count > 1){
    	    	MovePen(x + h * 4, y);
                newlen = ReadNextString(strp, len);
    			len -= newlen;
    			strp += newlen;
    			DrawTextString(s);
                MovePen(x + h * 5, y);
                newlen = ReadNextString(strp, len);
    			len -= newlen;
    			strp += newlen;
    			DrawTextString(s);
    	    }
    	    y -= 1.2 * h;
    	    Changexy();
        }
        Changexy();
        MovePen(x, y);
        newlen = ReadNextString(strp, len);
    	len -= newlen;
    	strp += newlen;
    	DrawTextString(s);
    	y -= 1.2 * h;
    	for(;;){
    	    MovePen(x, y);
            newlen = ReadNextString(strp, len);
            if(strcmp(s, "End") == 0){
            	len -= newlen;
    			strp += newlen;
            	break;
			}
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    	    y -= 1.2 * h;
    	    Changexy();
    	    MovePen(x, y);
    	    newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    	    MovePen(x + h * 2, y);
			newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		DrawTextString(s);
    		newlen = ReadNextString(strp, len);
    		len -= newlen;
    		strp += newlen;
    		int count = atoi(s); 
    	    if(count > 1){
    	    	MovePen(x + h * 4, y);
                newlen = ReadNextString(strp, len);
    			len -= newlen;
    			strp += newlen;
    			DrawTextString(s);
                MovePen(x + h * 5, y);
                newlen = ReadNextString(strp, len);
    			len -= newlen;
    			strp += newlen;
    			DrawTextString(s);
    	    }
    	    y -= 1.2 * h;
    	    Changexy();
        }
	    drawRectangle(xstart - 0.5 * h, ystart + h, 10 * h, y - ystart, 0);//分隔不同函数 
        y -= 1.2 * h;
    	if(y < winheight / 10){
			x = x + 11 * h;
        	y = winheight / 1.05;
		}
    }
    if(button(GenUIID(0), winwidth / 2.1, winheight / 40, 
 	   		  winwidth / 12, winheight / 30, "返回")){
		now = 1;
		textstate = NoChangePage;
		item = 1;
		*foldpath = '\0';
		*entrancefunc = '\0';
		globalstate = HomePage;
	}
}
