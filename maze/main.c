#include<stdio.h>
#include"io.h"
#include"solve.h"
#define map_size 14




int map[map_size][map_size]={0};
int path[map_size][map_size]={0};
int height , width;
int begin_x,begin_y,end_x,end_y;

int main (){

	read_map();
	solve();
	return 0;
}


