#ifndef TEXT_H
#define TEXT_H
int IsSpaceOrEnter(char c);//判断是否是回车和空格 
int IsLetterOrDightOrUnderline(char c);//判断是不是字符、数字和下划线（即命名的合理字符） 
void ChangeTextState(char *c);//改变文中行数和注释状态的属性,
int MyGetLine();//返回当前的行数
int FindNextChar(int start, char *s, int len, char c);//在长度为len字符串s从start位置开始寻找下一个合法的字符c并返回位置 
int IsLegalFunction(int start, int end, char *s);//判断是不是合法的函数 
int IsLegalState();//返回现在的状态是否是引用或者注释状态,不是则返回1 
int GetBracketDiffer();//获取现在左右括号差 
void TextInit();//初始化文件各个参数 
#endif
