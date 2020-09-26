/*
程序名：广度优先解决8数码问题 
作者：朱领 
时间：2020.9.25 
描述：
主要函数列表：
      1.init（）：输入初始状态和目标状态
	  2.loction（）：遍历找到空白棋子的位置
	  3.sign（）：将状态表示为一个数字唯一标识一个状态
	  4.mobile（）：上下左右移动状态
	  5.display（）：显示遍历的每一个状态
	  6.search（）：搜索状态，和目标状态对比
测试用例： 
// 初始状态： 
2 8 3
1 6 4
7 0 5

//目标状态： 
1 2 3
7 8 4
0 6 5

其它说明：此程序使用广度优先的算法， 
          (1): 如果保证可以达到目标节点，可以一直搜索下去，如果不知道能不能，就设置搜索次数 
*/


#include<stdio.h>


//****************利用节点表示状态********************
struct node{
	
	int status[3][3];
	
	//记录移动过的方向 
	int source;
};

/**************定义解空间大小************************
* (1).我这里设置了搜索次数，所以解空间大小设置成102，1存储初始状态，101存储目标状态
* （） 
*/

struct node st[102],end;
int count = 1; 


//********************init初始化方法*****************
void init()
{
	printf("输入起始状态3*3数组：\n");
	int i,j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			scanf("%d",&st[0].status[i][j]);
	st[0].source = -1; 
	
	printf("输入目标状态3*3数组：\n");
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			scanf("%d",&st[101].status[i][j]);
	st[101].source = -1; 
} 


//******************location：找出每个状态空白棋子位置并返回********

int loction(int num)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		if(st[num].status[i / 3][i % 3] == 0)return i; 
	} 
}

//******sign：将状态表示为一个9位数字唯一标识一个状态 *************
long long sign(int num)
{
	long long sum;
	sum = st[num].status[0][0]*100000000 + st[num].status[0][1]*10000000 + st[num].status[0][2]*1000000 + st[num].status[1][0]*100000 + st[num].status[1][1]*10000 + st[num].status[1][2]*1000 + st[num].status[2][0]*100 + st[num].status[2][1]*10 + st[num].status[2][2];
	return sum;
}

/***************mobile:移动元素，判断下一个***********************
* 规定source的0 1 2 3分别表示左上右下 
*/
void mobile(int num)
{
	int temp;
	//空白棋子位置 
	int loc; 
	int up = 1,down = 1,left = 1,right = 1;
	loc = loction(num);
	// 记录来源 
	int stand = st[num].source;
	 
	// （0）前一个状态左边移动到空白位置 
	if (loc % 3 != 2 && stand != 2)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3][loc % 3 + 1];
		st[count].status[loc / 3][loc % 3 + 1] = temp;
		st[count].source = 0;
		count++;
	}
 	
 	//（1）上边移动到空白位置
	if (loc / 3 != 2 && stand != 3)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3 + 1][loc % 3];
		st[count].status[loc / 3 + 1][loc % 3] = temp;
		st[count].source = 1;
		count++;
	}
	
	//（2）右边移动到空白位置
	if (loc % 3 != 0 && stand != 0)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3][loc % 3 - 1];
		st[count].status[loc / 3][loc % 3 - 1] = temp;
		st[count].source = 2;
		count++;
	}
	
	//（3)下边移动到空白位置 
	if (loc / 3 != 0 && stand != 1)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3 - 1][loc % 3];
		st[count].status[loc / 3 - 1][loc % 3] = temp;
		st[count].source = 3;
		count++;
	};
	
}
// ***************display：列出每一步状态*******************
void display(int num)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			printf("%d ", st[num].status[i][j]);
		printf("\n");
	}
}

//****************search: 搜索状态，和目标状态对比 *****
int search()
{
	int i = 0;
	while (1)
	{
		printf("\n");
		display(i);
		printf("\n");
		if (i == 100)
		{
			printf("超出了上限次数\n");
			return 0;
		}
		if (sign(i) == sign(101))
		{
			printf("在第%d次找到了\n", i);
			display(i);
			return i;
		}
		mobile(i);
		i++;
	}
}
int main()
{
	init();
	search();
	return 0;
}