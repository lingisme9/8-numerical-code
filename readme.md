# 8数码问题

## 问题描述

```
在3×3的棋盘上，摆有八个棋子，每个棋子上标有1至8的某一数字。棋盘中留有一个空格，空格用0来表示。空格周围的棋子可以移到空格中。要求解的问题是：给出一种初始布局（初始状态）和目标布局，找到一种最少步骤的移动方法，实现从初始布局到目标布局的转变。
```

## 问题分析

题目读完第一感觉是和求解最短路径问题类似，考虑使用BFS，状态很好找，每次移动空格就会形成一种新的状态，例如：

![img](https://gitee.com/lingisme9/typora/raw/master/img/20180116201941897)


![wXEZz6.png](https://s1.ax1x.com/2020/09/22/wXEZz6.png)

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

## 搜索方法的选择

### BFS（广度优先搜索）

广度优先遍历，指的是从图的一个未遍历的节点出发，先遍历这个节点的相邻节点，再依次遍历每个相邻节点的相邻节点。

上文所述树的广度优先遍历动图如下，每个节点的值即为它们的遍历顺序。所以广度优先遍历也叫层序遍历，先遍历第一层(节点 1)，再遍历第二层(节点 2，3，4)，第三层(5，6，7，8)，第四层(9，10)。

![img](https://gitee.com/lingisme9/typora/raw/master/img/29dc30c1546cc73846153359b0fad8fb.gif)

深度优先遍历用的是栈，而广度优先遍历要用队列来实现，我们以下图二叉树为例来看看如何用队列来实现广度优先遍历。

![img](https://gitee.com/lingisme9/typora/raw/master/img/1ba485c575477d00a51b44640671245e.png)

动图如下：

![img](https://gitee.com/lingisme9/typora/raw/master/img/a2c7c61edcadffeed85c10f53f1c988c.gif)

```
/** 
 * 使用队列实现 bfs 
 * @param root 
 */ 
private static void bfs(Node root) { 
    if (root == null) { 
        return; 
    } 
    Queue<Node> stack = new LinkedList<>(); 
    stack.add(root); 
 
    while (!stack.isEmpty()) { 
        Node node = stack.poll(); 
        System.out.println("value = " + node.value); 
        Node left = node.left; 
        if (left != null) { 
            stack.add(left); 
        } 
        Node right = node.right; 
        if (right != null) { 
            stack.add(right); 
        } 
    } 
} 


```

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

**2.DFS（深度优先搜索）**

主要思路是从图中一个未访问的顶点 V 开始，沿着一条路一直走到底，然后从这条路尽头的节点回退到上一个节点，再从另一条路开始走到底...，不断递归重复此过程，直到所有的顶点都遍历完成，它的特点是不撞南墙不回头，先走完一条路，再换一条路继续走。

1、我们从根节点 1 开始遍历，它相邻的节点有 2，3，4，先遍历节点 2，再遍历 2 的子节点 5，然后再遍历 5 的子节点 9。



![img](https://s4.51cto.com/oss/202004/16/ca375e4d1d4a377b4a15dc9e548123d1.png)

2、上图中一条路已经走到底了(9是叶子节点，再无可遍历的节点)，此时就从 9 回退到上一个节点 5，看下节点 5 是否还有除 9 以外的节点，没有继续回退到 2，2 也没有除 5 以外的节点，回退到 1，1 有除 2 以外的节点 3，所以从节点 3 开始进行深度优先遍历，如下：



![img](https://s3.51cto.com/oss/202004/16/2354c3d3f9abc912f6500140ee92458a.png)

3、同理从 10 开始往上回溯到 6, 6 没有除 10 以外的子节点，再往上回溯，发现 3 有除 6 以外的子点 7，所以此时会遍历 7。



![img](https://s2.51cto.com/oss/202004/16/7e0a19b7de2356a352661c4b0f803f71.png)

3、从 7 往上回溯到 3， 1，发现 1 还有节点 4 未遍历，所以此时沿着 4， 8 进行遍历,这样就遍历完成了。

完整的节点的遍历顺序如下(节点上的的蓝色数字代表)：

![92f602a1eaa301784085b8a3c4b948c0.png (381×337)](https://s5.51cto.com/oss/202004/16/92f602a1eaa301784085b8a3c4b948c0.png)

看到以上的遍历不难发现这就是树的前序遍历,实际上不管是前序遍历，还是中序遍历，亦或是后序遍历，都属于深度优先遍历。

那么深度优先遍历该怎么实现呢，有递归和非递归两种表现形式，接下来我们以二叉树为例来看下如何分别用递归和非递归来实现深度优先遍历。

**1、递归实现**

递归实现比较简单，由于是前序遍历，所以我们依次遍历当前节点，左节点，右节点即可，对于左右节点来说，依次遍历它们的左右节点即可，依此不断递归下去，直到叶节点(递归终止条件)，代码如下：

```
public class Solution { 
    private static class Node { 
        /** 
         * 节点值 
         */ 
        public int value; 
        /** 
         * 左节点 
         */ 
        public Node left; 
        /** 
         * 右节点 
         */ 
        public Node right; 
 
        public Node(int value, Node left, Node right) { 
            this.value = value; 
            this.left = left; 
            this.right = right; 
        } 
    } 
 
    public static void dfs(Node treeNode) { 
        if (treeNode == null) { 
            return; 
        } 
        // 遍历节点 
        process(treeNode) 
        // 遍历左节点 
        dfs(treeNode.left); 
        // 遍历右节点 
        dfs(treeNode.right); 
    } 
} 
```

递归的表达性很好，也很容易理解，不过如果层级过深，很容易导致栈溢出。所以我们重点看下非递归实现。

**2、非递归实现**

仔细观察深度优先遍历的特点，对二叉树来说，由于是先序遍历(先遍历当前节点，再遍历左节点，再遍历右节点)，所以我们有如下思路：

对于每个节点来说，先遍历当前节点，然后把右节点压栈，再压左节点(这样弹栈的时候会先拿到左节点遍历，符合深度优先遍历要求)。

弹栈，拿到栈顶的节点，如果节点不为空，重复步骤 1， 如果为空，结束遍历。

我们以以下二叉树为例来看下如何用栈来实现 DFS。

![b2a7d9fdc8fa1898a569255957428d94.png (355×287)](https://s2.51cto.com/oss/202004/16/b2a7d9fdc8fa1898a569255957428d94.png)

整体动图如下：

![e1e6a44251b69cd3b930f3071a71ffd8](https://s1.51cto.com/oss/202004/16/e1e6a44251b69cd3b930f3071a71ffd8.gif)

整体思路还是比较清晰的，使用栈来将要遍历的节点压栈，然后出栈后检查此节点是否还有未遍历的节点，有的话压栈，没有的话不断回溯(出栈)，有了思路，不难写出如下用栈实现的二叉树的深度优先遍历代码：

```
/** 
 * 使用栈来实现 dfs 
 * @param root 
 */ 
public static void dfsWithStack(Node root) { 
    if (root == null) { 
        return; 
    } 
 
    Stack<Node> stack = new Stack<>(); 
    // 先把根节点压栈 
    stack.push(root); 
    while (!stack.isEmpty()) { 
        Node treeNode = stack.pop(); 
        // 遍历节点 
        process(treeNode) 
 
        // 先压右节点 
        if (treeNode.right != null) { 
            stack.push(treeNode.right); 
        } 
 
        // 再压左节点 
        if (treeNode.left != null) { 
            stack.push(treeNode.left); 
        } 
    } 
} 
```

**深度优先实现8数码**

```
#include<cstdio>
#include<cstring>
#include<ctime>
char ans[11],start[10];
bool isUsed[11];
int changeId[9][4]={{-1,-1,3,1},{-1,0,4,2},{-1,1,5,-1},
					{0,-1,6,4},{1,3,7,5},{2,4,8,-1},
					{3,-1,-1,7},{4,6,-1,8},{5,7,-1,-1}
					};//0出现在0->8的位置后该和哪些位置交换 
const int M=400000;//9！=362800，因此数组开40W足够了 
int num[M],len=0,des=123804765;//num存储所有排列，len表示排列的个数也就是9！，des为目的状态直接用整数表示便于比较 
int isV[M][2];//bfs时判断状态是否出现过；isV的下标和num的下标一一对应，表示某种排列是否出现过
//通过isV和num建立起某种排列的组合成的整数int和bool的关系，其实STL中有map实现了key-->value，用排列作为key，value用bool即可 
int que1[M/2][3],que2[M/2][3];//0-->排列,1-->排列中0的位置,2-->步数 
void swap(char *c,int a,int b){//交换字符串中的两个位置 
	char t=c[a];
	c[a]=c[b];
	c[b]=t;
}
void paiLie(int n,int k){//深搜产生0-8的全排列 
	for(int i=0;i<n;i++){
		if(!isUsed[i]){
			ans[k]=i+'0';
			isUsed[i]=1;
			if(k==n){//已经有n个转换存储 
				ans[k+1]='\0';
				sscanf(ans+1,"%d",&num[len++]);
			}
			else
				paiLie(n,k+1);
			isUsed[i]=0;//回溯一步 
		}
	}
}
int halfFind(int l,int r,int n){//二分查找 
	int mid=l+(r-l)/2;
	if(num[mid]==n)return mid;
	else if(l<r&&num[mid]>n)return halfFind(l,mid-1,n);
	else if(l<r&&num[mid]<n) return halfFind(mid+1,r,n);
	return -1;
}
bool expand(int head,int &tail,int who,int q[][3]){
	char cur[10];//用于保存当前状态的字符串 
	int  pos=q[head][1],temp;//当前状态中0的位置
	sprintf(cur,"%09d",q[head][0]);//int-->char*这里的09d至关重要，否则算不出答案 
	for(int i=0;i<4;i++){//扩展当前的状态，上下左右四个方向 
		int swapTo=changeId[pos][i];//将要和那个位置交换 
		if(swapTo!=-1){//-1则不交换 
			swap(cur,pos,swapTo);//交换0的位置得到新状态 
			sscanf(cur,"%d",&temp);//新状态转换为int保存到temp 
			int k=halfFind(0,len,temp);//没有返回就查找当前排列的位置，将查出来的下标作为isV的下标 
			if(isV[k][0]==0){//如果 没有出现过，则将这个新状态进队 
				q[tail][0]=temp,q[tail][1]=swapTo,q[tail][2]=q[head][2]+1;
				isV[k][0]=who;
				isV[k][1]=tail;
				tail++;
			}
			else if(isV[k][0]&&isV[k][0]!=who){
				if(who==1)
					printf("%d", q[head][2]+que2[isV[k][1]][2]+1);
				else
					printf("%d", q[head][2]+que1[isV[k][1]][2]+1);
				return true;
			}
			swap(cur,pos,swapTo);//一个新状态处理完了一定要记得将交换的0交换回来 
		}
	}
	return false;
}
void bfs(int n,int p){
	int head1=0,tail1=1,head2=0,tail2=1;//head队头，tail队尾 
	que1[head1][0]=n,que1[head1][1]=p,que1[head1][2]=head1;//初始状态保存到对头，并设置当前步数为0 
	que2[head2][0]=des,que2[head2][1]=4,que2[head2][2]=head2;//初始状态保存到对头，并设置当前步数为0 
	int k=halfFind(0,len,n);
	isV[k][0]=1,isV[k][1]=0;
	k=halfFind(0,len,des);
	isV[k][0]=2,isV[k][1]=0;
	while(head1!=tail1||tail2!=head2){//队列不为空则继续搜索 
		if(tail2-head2>=tail1-head1){//2比1元素多就把1扩展 
			if(expand(head1,tail1,1,que1))return; 
			head1++;
		}
		else{
			if(expand(head2,tail2,2,que2))return; 
			head2++;
		}
	}
}
int main(){//812340756
	int n,i=-1,count=0;
	paiLie(9,1);//先将0-8的全排列按照升序产生出来存入num数组 
	scanf("%s",start);//输入初始状态 
	while(start[++i]!='0');//查找初始状态0的位置 
	sscanf(start,"%d",&n);//字符串转换为整数
	//int s=clock(); 
	if(n!=des)//判断输入状态是否就是目的状态 
		bfs(n,i); 
	else
		printf("%d",count);
	//printf("\n%.6lf",double(clock()-s)/CLOCKS_PER_SEC);
	return 0;
}

```


**3.启发式搜索**

  启发式搜索：启发式搜索就是在状态空间中的搜索对每一个搜索的位置进行评估，得到最好的位置，再从这个位置进行搜索直到目标。这样可以省略大量无谓的搜索路径，提高了效率



**解决此问题的启发策略：**

每次移动的时候，正确位置数码的个数要大于交换前正确位置数码个数。

正确位置数码个数：每个数码的位置与最终格局的对比，如果位置相同，则说明此数码在正确位置。



![wXe3FJ.png](https://s1.ax1x.com/2020/09/22/wXe3FJ.png)

图1.3中右边为最终格局，左边为当前格局，红色字体标识的数码为 正确位置数码，由此可以发现其正确位置的数码个数为4个。那么图1.2中正确数码如下图所示：

![wXeYS1.png](https://s1.ax1x.com/2020/09/22/wXeYS1.png)

由上图所示可得，正确位置数码个数大于等于4的只有左下方的格局，那么下一步选择的就是左下方的格局，再次调用次算法如下图：

![wXeg6P.png](https://s1.ax1x.com/2020/09/22/wXeg6P.png)




由八数码问题的部分状态图可以看出，从初始节点开始，在通向目标节点的路径上，各节点的数码格局同目标节点相比较，其数码不同的位置个数在逐渐减少，最后为零。所以，这个数码不同的位置个数便是标志一个节点到目标节点距离远近的一个启发性信息，利用这个信息就可以指导搜索。即可以利用启发信息来扩展节点的选择，减少搜索范围，提高搜索速度。

 启发函数设定。对于八数码问题，可以利用棋局差距作为一个度量。搜索过程中，差距会逐渐减少，最终为零，为零即搜索完成，得到目标棋局。



```
/*
程序名：八数码问题
描述：
主要函数列表：
      1.show()     显示当前待调整数码矩阵
	  2.exchange() 交换数码中的 begin[row_one][column_one] 与 begin[row_two][column_two] 这两个数
	  3.judge()    判断待调整的数码与最终数码相比正确位置数码的个数
      4.yidong()   将待调整数码从开始位置移动到终止位置，并将其过程输出
	  5.shuru()    有用户输入待调整的数码矩阵最初状态的数，并将其存入到begin[][]数组中
其它说明：此程序运用到启发式搜索的策略，
          （1）：将空格的地方存储零，这样便于操作
		  （2）：每次交换0上下左右4个方向元素的位置，当交换位置后正确数码个数 大于或等于 交换前正确数码的个数则算交换成功，
		         交换成功则重复步骤2，失败的话则跳回上一轮交换。
		  （3）：当交换到最终所有的数码的位置都正确时结束
*/
 
#include"stdio.h"
#define num 3 //宏定义数码的行列数为3
 
/*显示当前待调整数码矩阵*/
void show(int begin[num][num])  
{
	for(int i = 0; i < num; i++)
	{
		for(int j = 0; j < num; j++)
			printf("%d ", begin[i][j]);
		printf("\n");
	}
	printf("\n");
}
 
/*交换数码中的 begin[row_one][column_one] 与 begin[row_two][column_two] 这两个数*/
void exchange(int begin[num][num], int row_one, int column_one, int row_two, int column_two)  
{
	int temp;
	temp = begin[row_two][column_two] ;
	begin[row_two][column_two] = begin[row_one][column_one];
	begin[row_one][column_one] = temp;
}
 
/*判断待调整的数码与最终数码相比正确位置数码的个数*/
int judge(int begin[num][num], int end[num][num]) 
{
	int count=0;           //count记录数码中正确位置的个数
	for(int i = 0; i < num; i++)   //检查当前图形的正确度
		for(int j = 0; j < num; j++)
		{
			if(begin[i][j] == end[i][j] && end[i][j] != 0)
				count++;
		}
	return count;           //返回数码中正确位置的个数
}
 
/* 将待调整数码从开始位置移动到终止位置，并将其过程输出*/
int yidong(int begin[num][num], int end[num][num]
		   , int right, int jishu, int ji_shu[50][3][3]
		   , int biaoji, int row, int column) //biaoji存储上一轮移动的反方向代号
{ 
	int temp_zhi;
	show(begin);   //显示数组矩阵
	if(jishu >= 20)
		return 0;
	int node;  //,node为标记
	int temp;               //存储当前待调整数码正确的个数
    for(int q=0; q<jishu; q++)  //检查交换后的end[][]图形是否先前已经遍历过了
	{
		node = 1;
		for(int w=0; w<num && node; w++)
			for(int r=0; r<num && node; r++)
				if(ji_shu[q][w][r] != begin[w][r])
					node = 0;
		if(node == 1)   //如果先前遍历过，返回0
		{
	    	return 0;
		}
	}
	for(int i = 0; i < num; i++)   
		for(int j = 0; j < num; j++)
			ji_shu[jishu][i][j] = begin[i][j];
	
	if(right == num * num - 1)  //如果待调整数码与最终数码完全相同时，返回1
		return 1;
	if(row > 0 && biaoji != 0)             //存储0的位置不是在第一行
	{
		exchange(begin, row - 1, column, row , column);  //将0与其上面的元素交换存储位置
		temp = judge(begin, end);
	    if(temp < right)   //如果交换后正确数码的个数不大于原来正确数码的个数
	    	exchange(begin, row - 1, column, row , column); //再将其交换回来		
		else if(temp >= right)          //如果交换后正确数码的个数大于或等于原来正确数码的个数
		{
			temp_zhi = yidong(begin, end, temp, jishu+1, ji_shu, 2, row-1, column);
     		if( temp_zhi == 1)  //进行下一步的移动
	     	    return 1;
			exchange(begin, row - 1, column, row , column); //再将其交换回来
		}
	}
	if(column > 0 && biaoji != 1)
	{
	   	exchange(begin, row, column - 1, row , column); //将0与其左边的元素交换存储位置
		temp = judge(begin, end);		
		if(temp < right)   
   		   exchange(begin, row, column - 1, row , column);			
		else if(temp >= right)
		{
			temp_zhi = yidong(begin, end, temp, jishu+1, ji_shu ,3, row, column - 1);
		    if(temp_zhi == 1)  
                 return 1;
			exchange(begin, row, column - 1, row , column);
		}
	}
 
	if(row < num-1 && biaoji != 2)
	{
	    exchange(begin, row + 1, column, row , column); //将0与其下面的元素交换存储位置
		temp = judge(begin, end);	
		if(temp < right) 
			exchange(begin, row + 1, column, row , column);
		else if(temp >= right)
		{
			temp_zhi =yidong(begin, end, temp, jishu+1, ji_shu, 0, row+1, column);
		   if(temp_zhi == 1)  
		      return 1;
		   exchange(begin, row + 1, column, row , column);
		}
	}
	if(column < num-1 && biaoji != 3)
	{
	    exchange(begin, row, column + 1, row , column); //将0与其右边的元素交换存储位置
		temp = judge(begin, end);	
		if(temp < right)   
		    exchange(begin, row, column + 1, row , column);		
	    else if(temp >= right)  
		{
			temp_zhi = yidong(begin, end, temp, jishu+1, ji_shu, 1, row, column+1);
			if(temp_zhi == 1)  
		       return 1;
			exchange(begin, row, column + 1, row , column);	
		}
	}
	return 0;   //移动失败，返回0
}
 
/*有用户输入待调整的数码矩阵最初状态的数，并将其存入到begin[][]数组中*/
void shuru(int begin[][num],int blank[])  
{
	int temp, node, zero = 0;
	for (int i = 0; i < num; i++)
		for(int j = 0; j < num; j++)
		{
			node = 1;
			printf("请输入第%d行，第%d列的元素的值：", i+1, j+1);
		    scanf("%d", &temp);
			for (int q = 0; q <= i && node == 1; q++)  //当输入的值有重复的，提示重新输入
				for (int w = 0; w < j; w++)
					if(temp == begin[q][w])
					{
						printf("输入重复，请重新输入\n");
						node = 0;
						j--;
						break;
					}
			if(temp < 0 || temp > num*num-1)   //当输入的值不是在数码的区间范围内时，提示重新输入
			{
				printf("请输入从%d到%d的数\n", zero, num*num-1);
				node = 0;
			    j--;
			}
			if(node == 1)   //如果输入满足条件	
			{
				if(temp == 0) //如果输入的值为零，由blank[0]记录行号，blank[1]记录列号
				{
					blank[0] = i;
					blank[1] = j;
				}
				begin[i][j] = temp;//将满足条件的值存储起来
			}
		}
}
 
int main()
{
	int jishu = 0, ji_shu[50][3][3];//jishu存储已经遍历过的八数码图形的个数，jishu[][][]存储已经遍历过的八数码图形的形状
    int row;     //存储数字零的行数
    int column;  //存储数字零的列数
	int begin[num][num], blank[2],count=1; 	
	int end[num][num] = {1, 2, 3, 8, 0, 4, 7, 6, 5};  //给最终状态的数码矩阵赋值
    printf ("-------%d数码游戏开始！--------\n", num);
    shuru(begin, blank);   //输入带调整状态的数码矩阵的值
	row = blank[0];
	column = blank[1];
	if(yidong (begin, end,judge(begin,end),jishu,ji_shu,4,row,column) == 0)  
	   printf("\n此8数码的问题可能无解！");
	else 
	   show(begin);
	getchar();getchar();
	return 0;
}
```