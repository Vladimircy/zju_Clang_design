#ifndef LIST_H
#define LIST_H
//节点定义
struct Node{
	int exp; //指数
	int factor;//系数
	char sign;//符号
	struct Node *next;//节点指针（后驱指针）
};
typedef struct Node * Pnode;
Pnode init(Pnode node);
Pnode Trace(Pnode head, int exp);
Pnode Load(Pnode loader,char *s,int RESET);
Pnode Insert(Pnode target,Pnode loader);
Pnode Input(Pnode target,Pnode loader,char* s);
Pnode Get_list(Pnode head);
Pnode Mult_node(Pnode head,Pnode loader);
Pnode Mult(Pnode des, Pnode tar);
void List_free(Pnode head);
Pnode Merge(Pnode des, Pnode tar,int flag);
Pnode Copy_list(Pnode des , Pnode tar);
Pnode Copy_node(Pnode des , Pnode tar);
Pnode Display(Pnode head);
#endif
