#ifndef ANALYZE_H
#define ANALYZE_H
typedef struct{
	int id; //引用的id
	int line;//引用的行数 
}Call;
void Analyze(char *folderpath); 
int FindFuncByName(char *funcname);//输出函数名对应的Map的下标  
int FindFuncByRank(int rank);//输出对应排名的下标 
#endif
