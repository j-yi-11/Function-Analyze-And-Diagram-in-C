#ifndef TREAP_H
#define TREAP_H
#define lson tr[now].ls
#define rson tr[now].rs
#define MAXN 20010
//MAXN表示函数的最大数量
//本题平衡树用fhq_treap实现 
//可以应用root和tot分别表示树的根节点和节点总数 
struct value{
	char *funcname, *filename;
	int id, line; //定义的行数 
};//平衡树中的键值 
struct node{
	int size, ls, rs, pri;
	struct value val;
};//平衡树节点 
void TreapInit();//初始化一个treap 
void TreapInsert(struct value val);//插入一个数 
struct value TreapKth(int now, int k);//查找第k个数 
int findrank(struct value k);//找到键值k的排名 
struct value TreapFind(struct value k);//寻找与k函数名funcname相同的函数
#endif
