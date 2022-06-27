#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "treap.h"
#include "text.h"
#include "list.h"
#include "analyze.h"
#include "PutText.h"
/*#define DEBUG*/
extern int tot, root;
extern GlobalState globalstate, prevstate;
struct value Map[MAXN];//Map表示id到struct value之间的关系
int topcite[MAXN], topciter[MAXN];
List *call[MAXN], *caller[MAXN];//call里存引用的函数，caller里存引用它的函数
static int k1, k2, usedtimes;//用于FindFirstPreString函数的定位 
static void InitAllList(){
	int i;
	for(i = 1; i <= tot; i++)
	  call[i] = NewList(), caller[i] = NewList();
}
static void FindFirstPreString(char *file, int start){//找到start前的第一个非空字符串 
	k2 = start - 1;
	while(~k2 && IsSpaceOrEnter(file[k2])) k2--;//跳过中间的空格 
	k1 = k2;
	while(~k1 && IsLetterOrDightOrUnderline(file[k1])) k1--;//往前找到函数名 
	k1++;
}
static char* FindNextFunction(char *filename, char *file, int len){
	int i = 0, j, jj;
	i = FindNextChar(0, file, len, '('); //找到第一个左括号 
	if(i == len) return NULL;
	j = FindNextChar(i + 1, file, len, ')'); //找到左括号后的右括号
	jj = j + 1;
	while(jj < len && IsSpaceOrEnter(file[jj])) jj++;//找到下一个非空字符 
	if(jj == len) return NULL;
	if(file[jj] == '{'){//如果是)加上{就认为它是一个函数 
		FindFirstPreString(file, i); 
		if(!IsLegalFunction(k1, k2, file))
		  return file + j;
		struct value val;
		val.filename = calloc(strlen(filename), sizeof(char));
		strcpy(val.filename, filename);
		val.funcname = calloc(k2 - k1 + 2, sizeof(char));
		strncpy(val.funcname, file + k1, k2 - k1 + 1);
		val.funcname[k2 - k1 + 1] = 0; //防止calloc初始化不完全出现异常的乱码 
		val.line = MyGetLine();
		TreapInsert(val);
		val.id = tot;
		Map[tot] = val;
		#ifdef DEBUG 
		printf("%s %s %d\n", val.filename, val.funcname, val.line); 
		#endif
	}
	return file + j;
}
static char *FindNextFunctionUse(char *filename, char *file, int len){
	int i = 0, j, jj, l;
	i = FindNextChar(0, file, len, '('); //找到第一个左括号 
	if(i == len) return NULL;
	j = FindNextChar(i + 1, file, len, ')'); //找到左括号后的右括号
	jj = j + 1;
	while(jj < len && IsSpaceOrEnter(file[jj])) jj++;
	if(jj == len) return NULL;
	if(file[jj] == '{'){//jj指示左大括号的位置
		jj = j + 1;
		while(jj < len && IsSpaceOrEnter(file[jj])) ChangeTextState(file + jj), jj++;//更新一下状态
		ChangeTextState(file + jj), jj++; 
		FindFirstPreString(file, i); 
		if(!IsLegalFunction(k1, k2, file))
		  return file + (jj);
		struct value val, preval;
		val.filename = calloc(strlen(filename) + 1, sizeof(char));
		strcpy(val.filename, filename);
		val.filename[strlen(filename)] = 0;//防止calloc初始化不完全出现异常的乱码 
		val.funcname = calloc(k2 - k1 + 2, sizeof(char));
		strncpy(val.funcname, file + k1, k2 - k1 + 1);//用val存储现在的函数名和文件名 
		val.funcname[k2 - k1 + 1] = 0; //防止calloc初始化不完全出现异常的乱码 
		preval = TreapFind(val);//主要是为了获取id
//		free(val.filename);
//		free(val.funcname);//释放空间
		val = preval; 
		ChangeTextState(file + jj);
		jj++;
		Call *newcall = NULL, *newcaller = NULL;
		for(l = jj; GetBracketDiffer(); l++){//寻找直到左右括号匹配 
		   ChangeTextState(file + l);
		   if(!IsLegalState() || file[l] != '(') continue;
		   FindFirstPreString(file, l); 
		   struct value v, treapv;
		   v.filename = calloc(strlen(filename), sizeof(char));
		   strcpy(v.filename, filename);
		   v.funcname = calloc(k2 - k1 + 2, sizeof(char));
		   strncpy(v.funcname, file + k1, k2 - k1 + 1);
		   file[k2 - k1 + 1] = 0;
		   treapv = TreapFind(v);  //直接找到
//		   free(v.filename);
//		   free(v.funcname);//释放空间 
		   if(treapv.funcname == NULL) continue;//返回empty表示没有找到 
		   if(call[val.id]->tail != NULL){
				newcall = (Call *)(call[val.id]->tail->data);
		   		if(newcall->id == treapv.id && newcall->line == MyGetLine())//如果同一个函数的同行调用，就忽略 
		     		continue;
		   }
		   newcall = (Call *)malloc(sizeof(Call));
		   if(newcall == NULL){
		   	  printf("NULL POINTER");
		   	  exit(0);
		   }
		   newcall->id = treapv.id;
		   newcall->line = MyGetLine();
		   ListInsert(call[val.id], (void*)newcall, sizeof(Call));
		   newcaller = (Call *)malloc(sizeof(Call));
		   if(newcaller == NULL){
		   	  printf("NULL POINTER");
		   	  exit(0);
		   }
		   newcaller->id = val.id;
		   newcaller->line = MyGetLine();
		   ListInsert(caller[treapv.id], (void*)newcaller, sizeof(Call));
//		   free(newcall);
//		   free(newcaller);
		}
		return file + l;//如果是函数的话可以直接跳到整个函数的定义之后 
	}
	return file + j;
}
static void FindAllFunction(char *filename, char *file){
	TextInit();
	while(file = 
	FindNextFunction(filename, file, strlen(file)));
}
static void FindAllFunctionUse(char *filename, char *file){
	TextInit();
	while(file = 
	FindNextFunctionUse(filename, file, strlen(file))); 
}
static void Debug(int id){//输出Map中编号id的函数的调用关系以供调试 
	printf("%s %s %d\n", Map[id].filename, Map[id].funcname, Map[id].line);
	ListNode *j;
    printf("引用的函数：\n");
    for(j = call[id]->head; j != NULL; j = j->next){
    	Call *a = (Call *)(j->data);
    	printf("%s %s %d\n", Map[a->id].filename, Map[a->id].funcname, a->line);
    }
    printf("引用它的函数:\n");
    for(j = caller[id]->head; j != NULL; j = j->next){
    	Call *a = (Call *)(j->data);
    	printf("%s %s %d\n", Map[a->id].filename, Map[a->id].funcname, a->line);
    }
    printf("\n\n");
}
int FindFuncByName(char *funcname){
	struct value v, treapv;
	v.funcname = funcname;
	v.filename = NULL; 
	treapv = TreapFind(v);
	if(treapv.filename == NULL) return -1;
	return treapv.id;
}
int FindFuncByRank(int rank){
	return TreapKth(root, rank).id; 
}
char filestring[500000];
void FreeAll(){//清空所有的内存 
	int i;
	ListNode *pre, *j;
	for(i = 1; i <= tot; i++){
	  free(Map[i].filename);
	  free(Map[i].funcname);
	}
	for(i = 1; i <= tot; i++){
		for(j = call[i]->tail; j != NULL; j = pre){
		  pre = j->prev;
		  free(j);
		}
		free(call[i]);
	}
	for(i = 1; i <= tot; i++){
		for(j = caller[i]->tail; j != NULL; j = pre){
		  pre = j->prev;
		  free(j);
		}
		free(caller[i]);
	}
}
void FindFunction(char *folderpath){
	DIR *dp = NULL;
	FILE *fp = NULL;
 	struct dirent *Dirent;
 	struct stat filestate;
 	int ret = 0, cnt = 0;
 	char filename[1024] = {0};
 	dp = opendir(folderpath);
 	if(dp == NULL){
 	//	prevstate = globalstate;
        globalstate = NoFunction;
        return;
 	}
 	while(1){
  		Dirent = readdir(dp); //读取文件夹信息 
  		if(Dirent == NULL){//如果文件夹已经没有其他目录或者文件了 
   			break;
  		}
  		strcpy(filename, folderpath);
  		if(folderpath[strlen(folderpath) - 1] != '/') //判断路径名是否带/
   			strcat(filename,"/");
  		strcat(filename, Dirent->d_name); //新文件路径名
  		ret = stat(filename, &filestate); //查看目录下文件属性
  		if(ret < 0){
   			//prevstate = globalstate;
        	globalstate = NoFunction;
        	return;
  		}
  		if(S_ISDIR(filestate.st_mode)){//如果为目录文件
   			if(0 == strcmp("..", Dirent->d_name) || 0 == strcmp(".", Dirent->d_name)) //忽略当前目录和上一层目录
    			continue;
   			else
    			FindFunction(filename);  //递归读取
  		}
  		else{
   			//printf("%s\n",filename);
   			int len = strlen(filename);
   			if(len >= 2 && filename[len - 1] == 'c' && filename[len - 2] == '.'){//如果是.c文件 
   				fp = fopen(filename, "r");
            	cnt = 0;
            	while(!feof(fp)) filestring[cnt++] = getc(fp);
            	filestring[cnt++] = 0;
            	FindAllFunction(Dirent->d_name, filestring);
            	fclose(fp);
			}
  		}
	}
 	closedir(dp);
 	return ;
}
void FindFunctionUse(char *folderpath){
	DIR *dp = NULL;
	FILE *fp = NULL;
 	struct dirent *Dirent;
 	struct stat filestate;
 	int ret = 0, cnt = 0;
 	char filename[1024] = {0};
 	dp = opendir(folderpath);
 	if(dp == NULL){
 		//prevstate = globalstate;
        globalstate = NoFunction;
        return;
 	}
 	while(1){
  		Dirent = readdir(dp); //读取文件夹信息 
  		if(Dirent == NULL){//如果文件夹已经没有其他目录或者文件了 
   			break;
  		}
  		strcpy(filename, folderpath);
  		if(folderpath[strlen(folderpath) - 1] != '/') //判断路径名是否带/
   			strcat(filename,"/");
  		strcat(filename, Dirent->d_name); //新文件路径名
  		ret = stat(filename, &filestate); //查看目录下文件属性
  		if(ret < 0){
   			//prevstate = globalstate;
        	globalstate = NoFunction;
        	return;
  		}
  		if(S_ISDIR(filestate.st_mode)){//如果为目录文件
   			if(0 == strcmp("..", Dirent->d_name) || 0 == strcmp(".", Dirent->d_name)) //忽略当前目录和上一层目录
    			continue;
   			else
    			FindFunctionUse(filename);  //递归读取
  		}
  		else{
   			//printf("%s\n",filename);
   			int len = strlen(filename);
   			if(len >= 2 && filename[len - 1] == 'c' && filename[len - 2] == '.'){//如果是.c文件 
   				fp = fopen(filename, "r");
            	cnt = 0;
            	while(!feof(fp)) filestring[cnt++] = getc(fp);
            	filestring[cnt++] = 0;
            	FindAllFunctionUse(Dirent->d_name, filestring);
            	fclose(fp);
			}
  		}
	}
 	closedir(dp);
 	return ;
}
void Analyze(char *folderpath){
    if(usedtimes)
      FreeAll();
    //InitConsole();
    usedtimes++;
    TreapInit();
    FindFunction(folderpath);
    InitAllList();
    FindFunctionUse(folderpath);
}
