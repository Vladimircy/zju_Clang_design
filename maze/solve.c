#define map_size 14
#include"io.h"
#include <stdlib.h>
extern int map[map_size][map_size];
extern int path[map_size][map_size];
extern int height , width;
extern int begin_x,begin_y,end_x,end_y;
int end = 0;
int isavailable(int i,int j){
	if(path[i][j]||map[i][j])
		return 0;
	if(i<0||i>height-1||j<0||j>width-1)
		return 0;
	else
		return 1;
}
void dfs(int i,int j){
	if(i==end_y&&j==end_x){
		end = 1;
		output();
	}
	path[i][j]=1;
	if(isavailable(i,j+1)&&!end)
	dfs(i,j+1);
	if(isavailable(i+1,j)&&!end)
	dfs(i+1,j);
	if(isavailable(i,j-1)&&!end)
	dfs(i,j-1);
	if(isavailable(i-1,j)&&!end)
	dfs(i-1,j);
	path[i][j]=0;
	
}
void solve(void){
	dfs(begin_y,begin_x);
}
