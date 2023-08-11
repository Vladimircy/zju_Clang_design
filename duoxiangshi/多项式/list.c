#include"list.h"
#include"io.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define EXP_MAX 10000
//init 函数用于初始化链表中的一个节点，将指数，系数，符号，下一节点地址清空 并返回所给节点。
Pnode init(Pnode node){  
	node->factor = 0;
	node->exp = 0;
	node->sign = 0;
	node->next = NULL;
	return node;
}
// Trace 函数需要两个参数， 第一个为所给链表的头节点地址head，第二个为某一指数exp。
//函数用于找到该指数在head链表（按指数降序）中所对应的位置，并返回该一位置的前置（即指数大于等于exp的最后一节点）。
//函数实现：引入用于循环的指针tracer，比较tracer 和 tracer->next的exp 于所给exp的值。 
Pnode Trace(Pnode head, int exp){  
	Pnode tracer = head;
	while(tracer->next!=NULL){
		if(tracer->exp==exp)return tracer;  //find the same X^i
		if(head->next->exp<exp)return head; // find the largest X^i
		if(tracer->exp>exp&&tracer->next->exp<exp)return tracer;
		tracer = tracer->next;
	}
	return tracer;
}
//Load 函数需要三个参数，第一个为一个已分配内存的loader指针，一个指向存储多项式字符串的指针s 和模式调整RESET值；
//函数内置了一个static int的偏移量ind，用于记录已读取的位置，便于以项分割字符串；
//模式调整：RESET值为1 ，则将偏移量置为0，用来重新读取多项式字符串；
//读取链表实现方式：由于经过字符串预处理，每一项都可以以+-符号分割；在此基础上，需要再设置两个变量_x,_m;
//前者用于记录有没有出现X，后者用于记录有没有出现^;
//接下来，首先读取系数，以十进制方式读取每一个数字字符，在出现过^后，即存储指数；
//最后，需要处理两种特殊情况：1，X 出现而^未出现，此时指数为1；
//2， X出现而缺少前置系数，此时系数为1；
//从而，Load函数便可以记录多项式每一项。
Pnode Load(Pnode loader , char *s,int RESET){
	static int ind = 0;
	if(RESET == 1){
		ind = 0;
		return loader;
	}
	int _x = 0; //标记X
	int _m = 0; // 标记^
	if (s[ind]=='\0'){
		loader->sign = '\0';
		return loader;
	}
	loader->sign = s[ind];
	ind ++;
	while(s[ind]!='+'&&s[ind]!='-'&&ind<(int)strlen(s)){ //读到下一项的符号或者结尾
		if(s[ind]>='0'&&s[ind]<='9'){
			if (_x == 0) // 读入系数
				loader->factor =loader->factor*10+s[ind]-48; 
			else if(_m == 1)//读入指数
				loader->exp = loader->exp*10+s[ind]-48;
		}
		else if(s[ind] == 'X')_x = 1;
		else if (s[ind] == '^')_m = 1;
		ind++;
	}
	if(_x == 1 &&_m == 0)loader->exp = 1;   //特殊情况处理
	if(_x == 1 &&loader->factor==0 )loader->factor = 1;
	return loader;
}
//Insert函数用于插入节点，因此需要两个参数：目标链表插入位置的前置节点target 和 节点 loader；
//返回target。
Pnode Insert(Pnode target,Pnode loader){
	loader->next = target->next;
	target->next = loader;
	return target;
}
//Input函数采用递归方法创建链表，需要三个参数，链表头节点head，已存储某一项的loader节点和多项式字符串s；
//首先判断基线条件，即最后一项不存在，即符号值为0，则将这一loader free，并且返回链表头head；
//然后是递归部分，首先需要将这一项的loader插入到链表中，即先用Trace函数定位，随后采用相加或相减方式合并，或利用Insert函数插入；
//然后，读取下一节点，存储至new_loader中；
//继续递归，即可存储这一多项式至head中。
//返回值为传入head值。
Pnode Input(Pnode head,Pnode loader,char *s){
	if(loader->sign == '\0'){
		free(loader);
		return head;
	}		
	Pnode tracer = Trace(head,loader->exp);
	if(tracer->exp==loader->exp){
		if (loader->sign == '+') //符号问题
		tracer->factor += loader->factor;
		else
		tracer->factor -= loader->factor;
		free(loader);         //注意，这里需要free loader ，否则内存泄露；
	}
	else{
		if(loader->sign == '-') //符号问题，注意，在这一步处理中，factor的出现了正负之分，不在依赖于sign。
			loader->factor *=-1;
		Insert(tracer,loader);	
	}
	Pnode new_loader = (Pnode)malloc(sizeof(struct Node));
	new_loader = init(new_loader);
	Load(new_loader,s,0);
	Input(head,new_loader,s);
	return head;
	
}
//Mult_node用于将节点所对的项与链表所存的多项式相乘，返回所存储的head链表。
Pnode Mult_node(Pnode head,Pnode loader){
	Pnode tracer = head->next;
	while(tracer!= NULL){
		tracer->factor *= loader->factor;
		tracer->exp += loader->exp;
		tracer = tracer->next;
	}
	return head;
}
//Display函数用于展示head链表；
//处理输出需要应对多种情况：
//首项 ：符号问题，若+，则不输出符号，若-，则需要区分factor是否为-1，若是，则输出-，否则直接%d输出fator即可。
//中间项：符号问题，若factor>0则需要前置符号+；factor == -1，则需要输出-；
//        系数问题，factor 绝对值大于1，或该项为常数，则需要输出对应factor；
//        X的输出：X依赖于指数大于等于1以及系数不为0
//        ^的输出：^依赖于指数大于1以及系数不为0
//有无输出判断：用flag，记录有无输出，若无，则需要输出0；
Pnode Display(Pnode head){
	int flag = 0;
	Pnode tracer = head->next;
	while(tracer!=NULL){
		if(tracer!=head->next){
			if(tracer->factor>0){
			printf("+");
			flag = 1;
			}
		}
		if(abs(tracer->factor)>1||tracer->exp == 0){printf("%d",tracer->factor);
			flag = 1;
		}
		else if(tracer->factor==-1){
			printf("-");
			flag = 1;
		}
		if(tracer->exp>=1&&tracer->factor!=0){printf("X");
			flag = 1;
		}	
		if(tracer->exp>1&&tracer->factor!=0){printf("^%d",tracer->exp);
			flag = 1;
		}
		tracer = tracer->next;
	}
	if (flag == 0)printf("0\n");
	printf("\n");
	return head;
}
//List_free链表内存释放函数，传入一个节点，释放包括此节点和此节点后的所有节点的内存；
//该函数利用递归方法，先将tracer置于最后一节点，释放内存，随后回溯至上一节点，继续释放内存；
//最后将会free至所给节点；
void List_free(Pnode tracer){
	if(tracer->next==NULL){
		free(tracer);
		return ;
	}
	List_free(tracer->next);
	free(tracer);
}
//Copy_node函数，将所给tar节点复制到des节点（des节点已分配内存）
//注意，next值置空。
Pnode Copy_node(Pnode des , Pnode tar){
	des->factor = tar->factor;
	des->exp = tar->exp;
	des->sign = tar->sign;
	des->next = NULL;
	return des;
}
//Copy_list函数，复制链表。
//需要两个参数，des为已分配内存的头节点，tar为复制目标链表头节点；
//利用Copy_node和Insert实现。   
Pnode Copy_list(Pnode des , Pnode tar){
	Pnode tar_tracer = tar->next;
	Pnode des_tracer = des;
	while(tar_tracer!=NULL){
		Pnode node = (Pnode)malloc(sizeof(struct Node));
		node = Copy_node(node,tar_tracer);
		node->next = tar_tracer->next;
		Insert(des_tracer,node);
		des_tracer = des_tracer->next;
		tar_tracer = tar_tracer->next;
	}
	return des;
}
//Merge函数用于合并两个链表。
//需要三个参数，des为参与合并且生成合并后的链表，tar为参与合并的另一链表，flag区分加法合并和减法合并；
//采用了区间插入的思想，依据des中相邻节点的exp的值，判断tar中某一节点是否落在此区间内，或是与左端点值相等
//当判断完最有一组des区间后，可能有未在这一区间内的节点，则需要将这些节点加入到des中；
//最后，释放tar的内存，但保留了头节点，便于重复利用；
Pnode Merge(Pnode des , Pnode tar, int flag){
	Pnode des_tracer = des;
	Pnode tar_tracer = tar->next; 
	while(des_tracer->next!=NULL && tar_tracer!= NULL){ //与左端点相等，依据flag决定相加或是相减。
		if(des_tracer->exp== tar_tracer->exp){
			des_tracer->factor += flag*tar_tracer->factor;
			tar_tracer = tar_tracer->next;
		}
		else if(des_tracer->exp>tar_tracer->exp&&des_tracer->next->exp<tar_tracer->exp){//区间之内，需要插入该节点
			Pnode copy = (Pnode)malloc(sizeof(struct Node));
			copy = Copy_node(copy,tar_tracer); //注意，这里需要复制一个节点，以免原本链表被破坏；
			copy->factor *=flag; 
			Insert(des_tracer,copy);
			tar_tracer = tar_tracer->next;
		}
		des_tracer = des_tracer->next;
	} 
	while(tar_tracer!=NULL){  //处理区间外的值
		if(tar_tracer->exp!=des_tracer->exp){
			Pnode copy = (Pnode)malloc(sizeof(struct Node));
			copy = Copy_node(copy,tar_tracer);
			copy->factor *= flag;
			Insert(des_tracer,copy);
			des_tracer = des_tracer->next;
			tar_tracer = tar_tracer->next;
		}
		else{
			des_tracer->factor += flag*tar_tracer->factor;
			tar_tracer = tar_tracer->next;
		}
			
	}
	List_free(tar->next);
	tar->next = NULL;
	return des;
}
//Mult函数 用于将两个链表des和tar相乘，结果保留至另一链表res中，并返回；
//实现过程：
//多项式的乘法可以分解为其一节点乘以另一链表，随后将这些链表相加，得到结果；
//而链表与节点的乘法 和 链表与链表的合并，均已由Mult_node 和 Merge实现；
//因此，只需用临时变量存储中间多项式（存于temp中），在对其与res合并即可
Pnode Mult(Pnode des, Pnode tar){
	Pnode des_tracer = des->next;
	Pnode res = (Pnode)malloc(sizeof(struct Node));
	init(res);
	res->exp = EXP_MAX;
	while(des_tracer!=NULL){
		Pnode temp = (Pnode)malloc(sizeof(struct Node));
		init(temp);
		temp->exp = EXP_MAX;
		temp = Copy_list(temp,tar);
		res = Merge(res,Mult_node(temp,des_tracer),1); //注意，temp在Merge函数中已经释放了内存
		des_tracer = des_tracer->next;
	}
	List_free(des->next); //回收des和tar的内存，复用头节点
	List_free(tar->next);
	des->next=NULL;
	tar->next=NULL;
	return res;
}
//Get_list 是对Input和多项式字符串预处理函数read(定义在io.c中)的封装；
//只需传入头节点，即可获取链表；
Pnode Get_list(Pnode head){
	char* s = (char *)malloc(1000);
	s = read(s);
	Pnode loader = (Pnode)malloc(sizeof(struct Node));
	loader = init(loader);
	loader = Load(loader,s,1); //多余每一次新的多项式字符串，需要重置Load内置的偏移量ind；
	loader = Load(loader, s, 0);
	Input(head,loader,s);
	free(s);
	return head;
}
