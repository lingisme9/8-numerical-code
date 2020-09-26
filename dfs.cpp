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