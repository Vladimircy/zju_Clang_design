#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"
#include"frame.h"
#define EXP_MAX 10000
//Initialize为头节点初始化函数，为了便于插入，需要把头节点的exp置为较大值
Pnode Initialize(Pnode head){
	head = init(head);
	head->exp = EXP_MAX;
	return head;
}
//Begin_loop循环函数，根据所给选项（存于op中），用switch case，用相应函数处理多项式  
void Begin_loop(){
	creat_frame();
	int op;
	scanf("%d",&op);
	getchar();
	Pnode head = (Pnode)malloc(sizeof(struct Node));
	head = Initialize(head); //内置多项式
	Pnode target = (Pnode)malloc(sizeof(struct Node));
	target = Initialize(target);//输入的多项式，作用于内置多项式上
	while(op != 0){
		switch (op) {
		case 1:
			if(head->next!=NULL){  //若改变内置多项式，则需要先free内置多项式。
				List_free(head->next);
				head->next=NULL;
			}
			head = Get_list(head);
			break;
		case 2:
			target = Get_list(target);
			head = Merge(head,target,1);
			break;
		case 3:
			target = Get_list(target);
			head = Merge(head,target,-1);
			break;
		case 4:
			Get_list(target);
			head = Mult(head,target);
			break;
		case 5:
			Display(head);
			break;
		default:
			printf("Wrong number!");
			break;
		}
		creat_frame(); //重新打印界面
		scanf("%d",&op);
		getchar(); //读入回车符

	}
	if(op == 0){
		List_free(head);
		List_free(target);
		return ;}
}
//read为多项式字符串预处理函数，先读入一个多项式；
//若第一项无符号（默认为正），则加上+，若为负数，则无需处理
char * read(char *s){
	char *s1 = (char*)malloc(sizeof(1000+1));
	gets(s);
	if(s[0]!='-'){
		s1[0] = '+';
		s1[1] = '\0';
		s1 = strcat(s1,s);
	}
	else{
		s1[0] = '\0';
		s1 = strcat(s1 , s);
	}
	free(s);
	return s1;
}
