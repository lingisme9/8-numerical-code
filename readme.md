# 8数码问题

## 问题描述

```
在3×3的棋盘上，摆有八个棋子，每个棋子上标有1至8的某一数字。棋盘中留有一个空格，空格用0来表示。空格周围的棋子可以移到空格中。要求解的问题是：给出一种初始布局（初始状态）和目标布局，找到一种最少步骤的移动方法，实现从初始布局到目标布局的转变。
```

## 问题分析

题目读完第一感觉是和求解最短路径问题类似，考虑使用BFS，状态很好找，每次移动空格就会形成一种新的状态，例如：

![image-20200926091502520](https://gitee.com/lingisme9/typora/raw/master/img/image-20200926091502520.png)


![wXEZz6.png](https://gitee.com/lingisme9/typora/raw/master/img/wXEZz6.png)

八数码问题是个典型的状态图搜索问题。

搜索方式有两种基本的方式，即树式搜索和线式搜索。

搜索策略大体有盲目搜索和启发式搜索两大类。

​	**1.盲目搜索**就是无“向导”的搜索，（例如深度优先搜索，广度优先搜索），其效率极其低下。

​	**2.启发式搜索**就是有“向导”的搜索，有更好的效率

**一、状态如何表示？**

1.每个状态都用3*3的数组表示，但是BFS中需要入队出队，比较麻烦而且空间占用较大

2.**状态压缩**，采用一个整数保存状态的数字序列，例如状态1表示为283104765,状态2表示为203184765

**二、如何判重？**

1.正常需要876543210大小的数组，但是一般开不下来

2.虽然状态范围是012345678--876543210，但是中间真正有效的只有9！=362800，因为数字不可能出现重复；因此可以考虑开一个数组大小为9！整型数组A和bool数组B，然后生成0-8这9个数码的全排列并按照升序或者降序存入数组中，要判断某个状态（一种排列方式）是否出现过，直接通过二分查找的方式找到该排列在A中的下标i，然后查看数组B[i]为true还是false；如果为true则出现过，如果为false则将状态入队，并设置B[i]=true;

​	判重的实质就是建立状态数字串（一个int数据）和是否出现（一个bool数据）之间的联系，

3.将状态转换成9位的一个数字数字唯一标识一个状态

## 搜索方法的选择

### BFS（广度优先搜索）

广度优先遍历，指的是从图的一个未遍历的节点出发，先遍历这个节点的相邻节点，再依次遍历每个相邻节点的相邻节点。

上文所述树的广度优先遍历动图如下，每个节点的值即为它们的遍历顺序。所以广度优先遍历也叫层序遍历，先遍历第一层(节点 1)，再遍历第二层(节点 2，3，4)，第三层(5，6，7，8)，第四层(9，10)。

![img](https://gitee.com/lingisme9/typora/raw/master/img/29dc30c1546cc73846153359b0fad8fb.gif)

深度优先遍历用的是栈，而广度优先遍历要用队列来实现，我们以下图二叉树为例来看看如何用队列来实现广度优先遍历。

![img](https://gitee.com/lingisme9/typora/raw/master/img/1ba485c575477d00a51b44640671245e.png)

动图如下：

![img](https://gitee.com/lingisme9/typora/raw/master/img/a2c7c61edcadffeed85c10f53f1c988c.gif)

### **广度优先实现8数码**

**思路**

​	移动棋子最少，当然是使用广度优先搜索来解决。所以这里的每个节点就要是一个3*3的矩阵。如果在c语言中，可以使用结构体。

```
//****************利用节点表示状态********************
struct node{
	
	int status[3][3];

};
```

**实现方案**

- 空白棋子用0代替

- 接受初始节点的信息和目标节点的信息

- 找到空白棋子很简单，直接遍历就好，但是如何返回它的x和y坐标，试着能不能使用一个数字代替，后来发现确实可以。

- **广度优先搜索图**

  ![image-20200926093039024](https://gitee.com/lingisme9/typora/raw/master/img/image-20200926093039024.png)
  
  
  
  

```
//******************location：找出每个状态空白棋子位置并返回********

int loction(int num)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		if(st[num].status[i / 3][i % 3] == 0)return i; 
	} 
}
```

- 从初始节点开始判断，然后扩展，即上下左右移动，当然我们要考虑具体的位置，比如说已经到边界了，就不能越出边界。还要考虑以前移动过的方向，所以记录下来以前移动过的方向，可以直接加在结构体里。（代码如下所示）每次扩展的节点就加在数组后面。

  

```
//****************利用节点表示状态********************
struct node{
	
	int status[3][3];
	
	//记录移动过的方向 
	int source;
};

```

- 如何实现判断相等，就是如何标志该状态的唯一性，方案1：9个数字拼接生成字符串直接判断是否相等。方案二：由于数也不大，所以直接使用9个数字直接生成一个数，使用long long 声明生成的数字即可。

  

```
//******sign：将状态表示为一个9位数字唯一标识一个状态 *************
long long sign(int num)
{
	long long sum;
	sum = st[num].status[0][0]*100000000 + st[num].status[0][1]*10000000 + st[num].status[0][2]*1000000 + st[num].status[1][0]*100000 + st[num].status[1][1]*10000 + st[num].status[1][2]*1000 + st[num].status[2][0]*100 + st[num].status[2][1]*10 + st[num].status[2][2];
	return sum;
}
```

- 如果保证能够到达目标节点，可以一直搜索下去，如果不知道能不能，则可以设置搜索次数。

**代码框架**

![img](https://gitee.com/lingisme9/typora/raw/master/img/723452-20161102110956455-1290494945.jpg)

**具体代码实现**

```
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


```

测试结果：

![image-20200925224354052](https://gitee.com/lingisme9/typora/raw/master/img/image-20200925224354052.png)

![image-20200925224431685](https://gitee.com/lingisme9/typora/raw/master/img/image-20200925224431685.png)

### DFS（深度优先搜索）

主要思路是从图中一个未访问的顶点 V 开始，沿着一条路一直走到底，然后从这条路尽头的节点回退到上一个节点，再从另一条路开始走到底...，不断递归重复此过程，直到所有的顶点都遍历完成，它的特点是不撞南墙不回头，先走完一条路，再换一条路继续走。

1、我们从根节点 1 开始遍历，它相邻的节点有 2，3，4，先遍历节点 2，再遍历 2 的子节点 5，然后再遍历 5 的子节点 9。



![img](https://gitee.com/lingisme9/typora/raw/master/img/ca375e4d1d4a377b4a15dc9e548123d1.png)

2、上图中一条路已经走到底了(9是叶子节点，再无可遍历的节点)，此时就从 9 回退到上一个节点 5，看下节点 5 是否还有除 9 以外的节点，没有继续回退到 2，2 也没有除 5 以外的节点，回退到 1，1 有除 2 以外的节点 3，所以从节点 3 开始进行深度优先遍历，如下：



![img](https://gitee.com/lingisme9/typora/raw/master/img/2354c3d3f9abc912f6500140ee92458a.png)

3、同理从 10 开始往上回溯到 6, 6 没有除 10 以外的子节点，再往上回溯，发现 3 有除 6 以外的子点 7，所以此时会遍历 7。



![img](https://gitee.com/lingisme9/typora/raw/master/img/7e0a19b7de2356a352661c4b0f803f71.png)

3、从 7 往上回溯到 3， 1，发现 1 还有节点 4 未遍历，所以此时沿着 4， 8 进行遍历,这样就遍历完成了。

完整的节点的遍历顺序如下(节点上的的蓝色数字代表)：

![92f602a1eaa301784085b8a3c4b948c0.png (381×337)](https://gitee.com/lingisme9/typora/raw/master/img/92f602a1eaa301784085b8a3c4b948c0.png)

看到以上的遍历不难发现这就是树的前序遍历,实际上不管是前序遍历，还是中序遍历，亦或是后序遍历，都属于深度优先遍历。

那么深度优先遍历该怎么实现呢，有递归和非递归两种表现形式，接下来我们以二叉树为例来看下如何分别用递归和非递归来实现深度优先遍历。

**1、递归实现**

递归实现比较简单，由于是前序遍历，所以我们依次遍历当前节点，左节点，右节点即可，对于左右节点来说，依次遍历它们的左右节点即可，依此不断递归下去，直到叶节点(递归终止条件)

递归的表达性很好，也很容易理解，不过如果层级过深，很容易导致栈溢出。所以我们重点看下非递归实现。

**2、非递归实现**

仔细观察深度优先遍历的特点，对二叉树来说，由于是先序遍历(先遍历当前节点，再遍历左节点，再遍历右节点)，所以我们有如下思路：

对于每个节点来说，先遍历当前节点，然后把右节点压栈，再压左节点(这样弹栈的时候会先拿到左节点遍历，符合深度优先遍历要求)。

弹栈，拿到栈顶的节点，如果节点不为空，重复步骤 1， 如果为空，结束遍历。

我们以以下二叉树为例来看下如何用栈来实现 DFS。

![b2a7d9fdc8fa1898a569255957428d94.png (355×287)](https://gitee.com/lingisme9/typora/raw/master/img/b2a7d9fdc8fa1898a569255957428d94.png)

整体动图如下：

![7aa1e0a398d4b47db3773a802d990944](https://gitee.com/lingisme9/typora/raw/master/img/7aa1e0a398d4b47db3773a802d990944.gif)

整体思路还是比较清晰的，使用栈来将要遍历的节点压栈，然后出栈后检查此节点是否还有未遍历的节点，有的话压栈，没有的话不断回溯(出栈)

### 深度优先实现8数码

a) 把初始节点放入Open表中；
b) 如果Open表为空，则问题无解，失败退出；
c) 把Open表的第一个节点取出放入Close表，并标记该节点为n；
d) 考察n节点是否为目标节点。如果是，则得到问题的解，成功退出；
e) 如果节点n不可扩展，则转第b)步；
f) 扩展节点n，将其子节点放入Open表的头部，并为每一个子节点设置指向父亲节点的指针，然后转第b)步。

C++实现：

```
/*
程序名：深度度优先解决8数码问题 
作者：朱领 
时间：2020.9.26 
描述：
主要函数列表：
      1.IsGoal（）： 判断是否是目标状态
	  2.NextStatus（）：下一个可行的状态
	  3.Cantor（）：康托展开
	  4.DFS（）：深度优先搜索
	  5.PrintPath（）：显示遍历的每一个状态
	  6.freeCloseTable（）：释放close表
测试用例： 默认写在程序中了不用输入 
// 初始状态：
2 8 3
1 6 4
7 0 5

//目标状态： 
1 2 3
8 0 4
7 6 5

其它说明：此程序使用深度度优先的算法， 
          (1): 如果保证可以达到目标节点，可以一直搜索下去，如果不知道能不能，就设置搜索次数 
		  (2):可以在DFS下继续添加搜索方法 
*/
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

// 八数码状态
typedef struct _Status{
    int status[3][3];
    _Status *parent;
    _Status *next;
}Status;


// 八数码搜索
class EightPuzzle{
	private:
    	unsigned char allHash[362880];
    	Status root;
    	Status goal;
	private:
    	int nextNumber;
    	Status next[4];
	public:
    	EightPuzzle(Status *root, Status *goal){
        	memcpy(&this->root.status, &root->status, sizeof(int)*9);
        	this->root.parent = NULL;
        	this->root.next = NULL;
        	memcpy(&this->goal.status, &goal->status, sizeof(int)*9);
        	this->goal.parent = NULL;
        	this->goal.next = NULL;
    	}
	private:
    	// 判断是否是目标状态
    	inline int IsGoal(Status *tmp){
        	return memcmp(&tmp->status, &goal.status, sizeof(int)*9);
    	}
    	// 下一个可行的状态
    	int NextStatus(Status *tmp){
        	nextNumber = 0;
        	int posi, posj;
        	for(int i = 0; i < 9; i++){
            	posi = i/3, posj = i - i/3*3;
            	if(tmp->status[posi][posj] == 0){
                	break;
            	}
        }
        if(posi-1 >= 0){
            Status left = *tmp;
            left.status[posi][posj] = left.status[posi-1][posj];
            left.status[posi-1][posj] = 0;
            if(allHash[Cantor(left.status)] == 0){
                next[nextNumber] = left;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posi+1 <= 2){
            Status right = *tmp;
            right.status[posi][posj] = right.status[posi+1][posj];
            right.status[posi+1][posj] = 0;
            if(allHash[Cantor(right.status)] == 0){
                next[nextNumber] = right;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posj-1 >= 0){
            Status up = *tmp;
            up.status[posi][posj] = up.status[posi][posj-1];
            up.status[posi][posj-1] = 0;
            if(allHash[Cantor(up.status)] == 0){
                next[nextNumber] = up;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posj+1 <= 2){
            Status down = *tmp;
            down.status[posi][posj] = down.status[posi][posj+1];
            down.status[posi][posj+1] = 0;
            if(allHash[Cantor(down.status)] == 0){
                next[nextNumber] = down;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        return nextNumber;
    }
    // 康托展开
    int Cantor(int arr[][3]){
        int fac[10] = {1,1,2,6,24,120,720,5040,40320,362880};
        int index = 0;
        for(int i = 7; i >= 0; i--){
            int irow = i/3, icol = i - i/3*3;
            int count = 0;
            for(int j = 8; j > i; j--){
                int jrow = j/3, jcol = j - j/3*3;
                if(arr[jrow][jcol] < arr[irow][icol]){
                    count++;
                }
            }
            index += (count*fac[8-i]);
        }
        return index;
    }
	public:
    	// 深度优先搜索
    	int DFS(){
        	int depth = 0;
        	int step = 0;
        	stack<Status> openTable;
        	Status *closeTable = new Status;;
        	Status *current = closeTable;
        	Status *last;
        	Status *tmp;
        	openTable.push(root);
        	while(!openTable.empty()){
            	tmp = new Status;
            	*tmp = openTable.top();
            	openTable.pop();
            	step++;
            	current->next = tmp;
            	current = current->next;
            	if(IsGoal(tmp) == 0){
                	PrintPath(tmp);
                	freeCloseTable(closeTable);
                	return step;
            	}
            	memset(allHash, 0, 362880);
            	last = tmp;
            	depth = 0;
            	while(last != NULL){
                	allHash[Cantor(last->status)] = 1;
                	last = last->parent;
                	depth++;
            	}
            	if(depth > 8){
                	continue;
            	}
            	int nextNumber = NextStatus(tmp);
            	if(nextNumber == 0){
                	continue;
            	}
            	for(int i = 0; i < nextNumber; i++){
                	openTable.push(next[i]);
            	}
        	}
        	cout << "DFS failed." << endl;
        	freeCloseTable(closeTable);
        	return -1;
    	}
    
	private:
    	// 打印路径
    	void PrintPath(Status *head){
        	if(head == NULL){
            	return;
        	}
        	else{
            	PrintPath(head->parent);
            	for(int i = 0; i < 3; i++){
                	for(int j = 0; j < 3; j++){
                    	cout << head->status[i][j];
                	}
                	cout << endl;
            	}
            	cout <<endl;
        	}
    	}
    	// 释放close表
    	void freeCloseTable(Status *closeTable){
        	Status *current;
        	while(closeTable != NULL){
            	current = closeTable->next;
            	free(closeTable);
            	closeTable = current;
        	}
    	}
	};

int main()
{
    Status init = {2,8,3,1,6,4,7,0,5,0,NULL};
    Status goal = {1,2,3,8,0,4,7,6,5,0,NULL};
    EightPuzzle ep = EightPuzzle(&init, &goal);
    cout << "***********\n" << endl;
    cout << "DFS********\n" << endl;
    cout << "step: " << ep.DFS() << endl;
    cout << "***********\n" << endl;
    return 0;
}
```



### 启发式搜索实现8数码

  启发式搜索：启发式搜索就是在状态空间中的搜索对每一个搜索的位置进行评估，得到最好的位置，再从这个位置进行搜索直到目标。这样可以省略大量无谓的搜索路径，提高了效率



**解决此问题的启发策略：**

每次移动的时候，正确位置数码的个数要大于交换前正确位置数码个数。

正确位置数码个数：每个数码的位置与最终格局的对比，如果位置相同，则说明此数码在正确位置。



![wXe3FJ.png](https://gitee.com/lingisme9/typora/raw/master/img/wXe3FJ.png)

图1.3中右边为最终格局，左边为当前格局，红色字体标识的数码为 正确位置数码，由此可以发现其正确位置的数码个数为4个。那么图1.2中正确数码如下图所示：

![wXeYS1.png](https://gitee.com/lingisme9/typora/raw/master/img/wXeYS1.png)

由上图所示可得，正确位置数码个数大于等于4的只有左下方的格局，那么下一步选择的就是左下方的格局，再次调用次算法如下图：

![wXeg6P.png](https://gitee.com/lingisme9/typora/raw/master/img/wXeg6P.png)




由八数码问题的部分状态图可以看出，从初始节点开始，在通向目标节点的路径上，各节点的数码格局同目标节点相比较，其数码不同的位置个数在逐渐减少，最后为零。所以，这个数码不同的位置个数便是标志一个节点到目标节点距离远近的一个启发性信息，利用这个信息就可以指导搜索。即可以利用启发信息来扩展节点的选择，减少搜索范围，提高搜索速度。

 启发函数设定。对于八数码问题，可以利用棋局差距作为一个度量。搜索过程中，差距会逐渐减少，最终为零，为零即搜索完成，得到目标棋局。

启发式搜索的算法如下：
a) 把初始节点放入Open表中，计算其f值；
b) 如果Open表为空，则问题无解，失败退出；
c) 把Open表的第一个节点取出放入Close表，并标记该节点为n；
d) 考察n节点是否为目标节点。如果是，则得到问题的解，成功退出；
e) 如果节点n不可扩展，则转第b)步；
f) 扩展节点n，计算每一个子节点的f值，并为每个子节点设置指向节点n的指针，将这些子节点放入Open表中；
g) 根据各节点的f值，对Open表中给的全部节点按照从小到大的顺序排序；
h) 转第b)步。

```
/*
程序名：A*解决8数码问题 
作者：朱领 
时间：2020.9.26
描述：
主要函数列表：
      1.IsGoal（）： 判断是否是目标状态
	  2.NextStatus（）：下一个可行的状态
	  3.Cantor（）：康托展开
	  4.Atart（）：A*搜索 
	  5.PrintPath（）：显示遍历的每一个状态
	  6.freeCloseTable（）：释放close表
	  7.decComparator():A*排序依据
测试用例： 默认写在程序中了不用输入 
// 初始状态：
2 8 3
1 6 4
7 0 5

//目标状态： 
1 2 3
8 0 4
7 6 5

其它说明：此程序使用深度度优先的算法， 
          (1): 如果保证可以达到目标节点，可以一直搜索下去，如果不知道能不能，就设置搜索次数 
		  (2):可以在DFS下继续添加搜索方法 
		  (3):继承了深度优先的程序结构，直接加入Astar算法
*/
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

// 八数码状态
typedef struct _Status{
    int status[3][3];
    _Status *parent;
    _Status *next;
}Status;

// AStar排序依据
bool decComparator(const Status &s1, const Status &s2){
    int gn1 = 0, gn2 = 0;
    int dn1 = 0, dn2 = 0;
    const Status *ptr1 = &s1;
    const Status *ptr2 = &s2;
    int status[3][3] = {1,2,3,8,0,4,7,6,5};
    while(ptr1 != NULL){
        gn1 += 1;
        ptr1 = ptr1->parent;
    }
    while(ptr2 != NULL){
        gn2 += 1;
        ptr2 = ptr2->parent;
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(s1.status[i][j] != status[i][j]){
                dn1 += 1;
            }
            if(s2.status[i][j] != status[i][j]){
                dn2 += 1;
            }
        }
    }
    return (gn1+dn1) > (gn2+dn2);
}

// 八数码搜索
class EightPuzzle{
private:
    unsigned char allHash[362880];
    Status root;
    Status goal;
private:
    int nextNumber;
    Status next[4];
public:
    EightPuzzle(Status *root, Status *goal){
        memcpy(&this->root.status, &root->status, sizeof(int)*9);
        this->root.parent = NULL;
        this->root.next = NULL;
        memcpy(&this->goal.status, &goal->status, sizeof(int)*9);
        this->goal.parent = NULL;
        this->goal.next = NULL;
    }
private:
    // 判断是否是目标状态
    inline int IsGoal(Status *tmp){
        return memcmp(&tmp->status, &goal.status, sizeof(int)*9);
    }
    // 下一个可行的状态
    int NextStatus(Status *tmp){
        nextNumber = 0;
        int posi, posj;
        for(int i = 0; i < 9; i++){
            posi = i/3, posj = i - i/3*3;
            if(tmp->status[posi][posj] == 0){
                break;
            }
        }
        if(posi-1 >= 0){
            Status left = *tmp;
            left.status[posi][posj] = left.status[posi-1][posj];
            left.status[posi-1][posj] = 0;
            if(allHash[Cantor(left.status)] == 0){
                next[nextNumber] = left;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posi+1 <= 2){
            Status right = *tmp;
            right.status[posi][posj] = right.status[posi+1][posj];
            right.status[posi+1][posj] = 0;
            if(allHash[Cantor(right.status)] == 0){
                next[nextNumber] = right;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posj-1 >= 0){
            Status up = *tmp;
            up.status[posi][posj] = up.status[posi][posj-1];
            up.status[posi][posj-1] = 0;
            if(allHash[Cantor(up.status)] == 0){
                next[nextNumber] = up;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        if(posj+1 <= 2){
            Status down = *tmp;
            down.status[posi][posj] = down.status[posi][posj+1];
            down.status[posi][posj+1] = 0;
            if(allHash[Cantor(down.status)] == 0){
                next[nextNumber] = down;
                next[nextNumber].parent = tmp;
                nextNumber++;
            }
        }
        return nextNumber;
    }
    // 康托展开
    int Cantor(int arr[][3]){
        int fac[10] = {1,1,2,6,24,120,720,5040,40320,362880};
        int index = 0;
        for(int i = 7; i >= 0; i--){
            int irow = i/3, icol = i - i/3*3;
            int count = 0;
            for(int j = 8; j > i; j--){
                int jrow = j/3, jcol = j - j/3*3;
                if(arr[jrow][jcol] < arr[irow][icol]){
                    count++;
                }
            }
            index += (count*fac[8-i]);
        }
        return index;
    }
public:
    // 启发式搜索
    int AStar(){
        int step = 0;
        memset(allHash, 0, 362880);
        vector<Status> openTable;
        Status *closeTable = new Status;;
        Status *current = closeTable;
        Status *tmp;
        openTable.push_back(root);
        allHash[Cantor(root.status)] == 1;
        while(!openTable.empty()){
            tmp = new Status;
            *tmp = openTable[openTable.size()-1];
            openTable.pop_back();
            step++;
            current->next = tmp;
            current = current->next;
            if(IsGoal(tmp) == 0){
                PrintPath(tmp);
                freeCloseTable(closeTable);
                return step;
            }
            int nextNumber = NextStatus(tmp);
            if(nextNumber == 0){
                continue;
            }
            for(int i = 0; i < nextNumber; i++){
                openTable.push_back(next[i]);
                allHash[Cantor(next[i].status)] == 1;
            }
            sort(openTable.begin(), openTable.end(), decComparator);
        }
        cout << "AStar failed." << endl;
        freeCloseTable(closeTable);
        return -1;
    }
private:
    // 打印路径
    void PrintPath(Status *head){
        if(head == NULL){
            return;
        }
        else{
            PrintPath(head->parent);
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    cout << head->status[i][j];
                }
                cout << endl;
            }
            cout <<endl;
        }
    }
    // 释放close表
    void freeCloseTable(Status *closeTable){
        Status *current;
        while(closeTable != NULL){
            current = closeTable->next;
            free(closeTable);
            closeTable = current;
        }
    }
};

int main()
{
    Status init = {2,8,3,1,6,4,7,0,5,0,NULL};
    Status goal = {1,2,3,8,0,4,7,6,5,0,NULL};
    EightPuzzle ep = EightPuzzle(&init, &goal);
    cout << "***********\n" << endl;
    cout << "AStar******\n" << endl;
    cout << "step: " << ep.AStar() << endl;
    cout << "***********\n" << endl;
    return 0;
}
```