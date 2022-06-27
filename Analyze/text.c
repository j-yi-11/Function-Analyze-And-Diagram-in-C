#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int Line, AnnotationState = 0, QuoteState = 0, BracketDiffer = 0;
//Line表示当前文件的行数，AnnotationState是否是注释, QuoteState表示是否处于引用之中,BracketDiffer表示左括号和右括号之差 
int IsSpaceOrEnter(char c){
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
int IsLetterOrDightOrUnderline(char c){
	return (c >= '0' && c <= '9') || (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c <= 'Z' && c >= 'A') || c == '_';
}
//ChangeLine和ChangeAnnotationState都需要传入文件指针 
static void ChangeLine(char *c){
	if(((*c) == '\n' && (*(c - 1)) != '\r') || (*c) == '\r') //考虑Windows系统下换行为'\r\n',Unix'\n',Max'\r' 
	  Line++;
}
static void ChangeAnnotationState(char *c){
	if(QuoteState) return;//如果是在注释之中，直接忽略 
	if(*c == '/' && *(c - 1) == '/'){
	  if(AnnotationState == 0) 
	    AnnotationState = 1;
	}
	else if(*c == '/' && *(c - 1) == '*'){
	  if(AnnotationState == 2)
	    AnnotationState = 0;
	}
	else if(*c == '*' && *(c - 1) == '/'){
	  if(AnnotationState == 0)
	    AnnotationState = 2;
    }
    else if(((*c == '\n' && *(c - 1) != '\r') || *c == '\r')){
    	if(AnnotationState == 1)
    	  AnnotationState = 0;
	}
}
static void ChangeQuoteState(char *c){
	if(AnnotationState) return;//如果现在在注释中就忽略 
	if(*c == 34){//双引号 
		if(QuoteState == 0) QuoteState = 1;
		else if(QuoteState == 1) QuoteState = 0;
	}
	else if(*c == 39){//单引号
		if(QuoteState == 0) QuoteState = 2;
		else if(QuoteState == 2) QuoteState = 0;
	}
}
static void ChangeBracketState(char *c){
	if(AnnotationState || QuoteState) return;
	if(*c == '{') BracketDiffer++;
	else if(*c == '}') BracketDiffer--;
} 
int IsLegalState(){
	return !AnnotationState && !QuoteState;
}
void ChangeTextState(char *c){
	ChangeLine(c);
	ChangeAnnotationState(c);
	ChangeQuoteState(c);
	ChangeBracketState(c);
}
int MyGetLine(){
	return Line;
}
int GetBracketDiffer(){
	return BracketDiffer;
}
void TextInit(){
	Line = 1;
	AnnotationState = 0;
	QuoteState = 0;
	BracketDiffer = 0; 
}
static int JumpOutAnnotation(int start, char *s, int len){//跳出注释部分
	int i = start;
	while(i < len && AnnotationState)
		ChangeTextState(s + i), i++;
	return i;
}
static int JumpOutQuote(int start, char *s, int len){//跳出引用部分
	int i = start;
	while(i < len && QuoteState)
		ChangeTextState(s + i), i++;
	return i;
}
int FindNextChar(int start, char *s, int len, char c){
	int i = start;
	while(i < len && s[i] != c){
	  ChangeTextState(s + i), i++; //指示(的位置
	  if(AnnotationState > 0)
	    i = JumpOutAnnotation(i, s, len);
	  if(QuoteState > 0)
	    i = JumpOutQuote(i, s, len);
    }
    return i;
}
int IsLegalFunction(int start, int end, char *s){
	if(end - start == 1 && s[start] == 'i' && s[end] == 'f')//去掉if条件判断的干扰 
		return 0;
	if(end - start == 2 && s[start] == 'f' && s[start + 1] == 'o' && s[end] == 'r')//去掉for 
		return 0;
	if(end - start == 4 && s[start] == 'w' && s[start + 1] == 'h' && s[start + 2] == 'i' && s[start + 3] == 'l' && s[end] == 'e')//去掉while
		return 0;
	if(end - start == 1 && s[start] == 'd' && s[end] == 'o')//去掉do 
		return 0;
	if(end - start == 5 && s[start] == 's' && s[start + 1] == 'w' && s[start + 2] == 'i' && s[start + 3] == 't' && s[start + 4] == 'c' && s[start + 5] == 'h')//去掉switch 
	    return 0;
	return 1;
}
