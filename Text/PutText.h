#ifndef PUTTEXT_H
#define PUTTEXT_H
void Changexy();
void Page();
void PutText();
void TurnPage();
void TextPlay();
void PageButton();
void ReadResult();
typedef enum{
	HomePage = 0,
	TextEnterPath = 1,
	TextFunction = 2,
	ShowHelp = 3,
	StoreFile = 4,
	NoFunction = 5,
	OverSize = 6, 
	OneFunctionDiagram = 7,
	DiagramEnterPath = 8,
	FunctionNotFound = 9,
	ReadFromFile = 10, 
}GlobalState;
/*定义枚举类型，代表现在绘图的状态
0表示主页 
1表示以文本形式输出调用情况前的路径输入 
2表示以文本形式输出调用情况
3表示显示help
4表示示保存分析信息到文件
5表示当前目录没有C文件 
6表示函数数量过多 
7表示单独绘制某个函数的调用图
8表示单独绘制某个函数的调用图前的路径输入 
9表示没有找到该函数
10表示从文件里读取数据 
*/
typedef enum{
	NoChangePage = 0,
	NextPage = 1,
	PrevPage = 2,
}TextState;
/*
0表示维持原状
1表示下一页
2表示上一页 
*/
#endif
