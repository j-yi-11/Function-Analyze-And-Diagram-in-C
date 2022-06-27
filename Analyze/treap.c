#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "treap.h"
int tot, root;
struct value empty;
struct node tr[MAXN << 1];//treap的节点 
int mystrcmp(char *s1, char *s2){
	if(s1 == NULL || s2 == NULL){
		if(s1 == s2) return 0;
		return s1 == NULL ? -1 : 1;
	}
	//printf("%s %s %d\n", s1, s2, strcmp(s1, s2));
	return strcmp(s1, s2);
}
static int cmp(struct value v1, struct value v2){//定义字符串大小s1 < s2的比较函数;
	if((mystrcmp(v1.funcname, v2.funcname) == 0) && (mystrcmp(v1.filename, v2.filename) == 0)) return 0;//函数名和文件名相同就认为完全相同 
	return (mystrcmp(v1.funcname, v2.funcname) < 0) 
	|| ((mystrcmp(v1.funcname, v2.funcname) == 0) && (mystrcmp(v1.filename, v2.filename) < 0));
}
static void pushup(int now){//更新父节点的变量值 
	tr[now].size = tr[lson].size + tr[rson].size + 1;
}
static void split(int now, struct value k, int *x, int *y){//按照k值分裂,x为分裂出的左子树其中元素<=k，y为分裂出来的右子树，now是现在的节点，本质是在树上二分 
	if(!now){
		*x = *y = 0;
		return;
	}
	if(cmp(k, tr[now].val)){ //现在节点的值如果比k大，就在左子树继续找 
		*y = now;
		split(lson, k, x, &lson);
	}
	else{ //现在节点如果相等或者比k小，就往右子树继续找 
		*x = now;
		split(rson, k, &rson, y);
	}
	pushup(now);
}
static int merge(int x, int y){//返回合并后的节点编号 
	if(!x || !y) return x + y;
	if(tr[x].pri > tr[y].pri){//按照优先级合并 
		tr[x].rs = merge(tr[x].rs, y);
		pushup(x);
		return x;
	}
	else{
		tr[y].ls = merge(x, tr[y].ls);
		pushup(y);
		return y;
	}
}
void TreapInsert(struct value val){
	tr[++tot].val = val;
	tr[tot].val.id = tot;
	tr[tot].size = 1;
	tr[tot].pri = rand();
	int x, y;
	split(root, val, &x, &y);
	root = merge(merge(x, tot), y);
}
struct value TreapKth(int now, int k){
	for(; now;){
	    if(tr[lson].size + 1 == k) return tr[now].val;
		else if(k <= tr[lson].size) now = lson;
		else k -= tr[lson].size + 1, now = rson;
	}
	return empty;
}
int findrank(struct value k){
	int x, y;
	split(root, k, &x, &y);
	int sum = tr[x].size;
	root = merge(x, y);
	return sum;//否则将从0开始 
}
struct value TreapFind(struct value k){//找到小于它的第一个函数 
	//printf("%d\n", findrank(k)); 
	//return kth(root, findrank(k));
	int x, y;
	split(root, k, &x, &y);
	int now = x;
	while(rson)
	  now = rson;
	struct value v1 = tr[now].val;
	now = y;
	while(lson)
	  now = lson;
	struct value v2 = tr[now].val;
	root = merge(x, y);
	if(mystrcmp(v1.funcname, k.funcname) == 0) return v1;
	if(mystrcmp(v2.funcname, k.funcname) == 0) return v2;
	return empty;
}
void TreapInit(){
	empty.filename = NULL, empty.funcname = NULL;
	root = tot = 0;
	int i;
	for(i = 0; i < MAXN << 1; i++)
	  tr[i].ls = tr[i].rs = tr[i].pri = tr[i].size = 0;
} 
