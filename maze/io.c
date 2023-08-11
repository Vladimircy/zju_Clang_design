#include<stdio.h>
#define map_size 14
extern int map[map_size][map_size];
extern int path[map_size][map_size];
extern int height , width;
extern int begin_x,begin_y,end_x,end_y;
void output(){
	printf("\n\n\n");
	for(int i = 0;i < height; i++){
		for(int j = 0; j < width; j++){
			if(map[i][j]==1)
				printf("%c",'#');
			else if (j == begin_x&&i==begin_y)
				printf("$");
			else if(j==end_x&&i==end_y)
				printf("&");
			else if(path[i][j]==1)
				printf("%c",'*');
			else
				printf(" ");
		}
		printf("\n");
	}
}
void read_map(void){
	scanf("%d%d",&height,&width);
	getchar();
	for(int i = 0;i < height; i++){
		for(int j = 0; j < width; j++){
			char c ;
			if((c = getchar()) == '#')
				map[i][j] = 1;
			else if(c ==' ')
				map[i][j] = 0;
			else if (c == '$')
				begin_x = j,begin_y = i;
			else if (c == '&')
				end_x = j ,end_y = i; 
			else printf("输入错误！");
		}
		getchar();
	}
}



