/*
��������������Ƚ��8�������� 
���ߣ����� 
ʱ�䣺2020.9.25 
������
��Ҫ�����б�
      1.init�����������ʼ״̬��Ŀ��״̬
	  2.loction�����������ҵ��հ����ӵ�λ��
	  3.sign��������״̬��ʾΪһ������Ψһ��ʶһ��״̬
	  4.mobile���������������ƶ�״̬
	  5.display��������ʾ������ÿһ��״̬
	  6.search����������״̬����Ŀ��״̬�Ա�
���������� 
// ��ʼ״̬�� 
2 8 3
1 6 4
7 0 5

//Ŀ��״̬�� 
1 2 3
7 8 4
0 6 5

����˵�����˳���ʹ�ù�����ȵ��㷨�� 
          (1): �����֤���ԴﵽĿ��ڵ㣬����һֱ������ȥ�������֪���ܲ��ܣ��������������� 
*/


#include<stdio.h>


//****************���ýڵ��ʾ״̬********************
struct node{
	
	int status[3][3];
	
	//��¼�ƶ����ķ��� 
	int source;
};

/**************�����ռ��С************************
* (1).�������������������������Խ�ռ��С���ó�102��1�洢��ʼ״̬��101�洢Ŀ��״̬
* ���� 
*/

struct node st[102],end;
int count = 1; 


//********************init��ʼ������*****************
void init()
{
	printf("������ʼ״̬3*3���飺\n");
	int i,j;
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			scanf("%d",&st[0].status[i][j]);
	st[0].source = -1; 
	
	printf("����Ŀ��״̬3*3���飺\n");
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			scanf("%d",&st[101].status[i][j]);
	st[101].source = -1; 
} 


//******************location���ҳ�ÿ��״̬�հ�����λ�ò�����********

int loction(int num)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		if(st[num].status[i / 3][i % 3] == 0)return i; 
	} 
}

//******sign����״̬��ʾΪһ��9λ����Ψһ��ʶһ��״̬ *************
long long sign(int num)
{
	long long sum;
	sum = st[num].status[0][0]*100000000 + st[num].status[0][1]*10000000 + st[num].status[0][2]*1000000 + st[num].status[1][0]*100000 + st[num].status[1][1]*10000 + st[num].status[1][2]*1000 + st[num].status[2][0]*100 + st[num].status[2][1]*10 + st[num].status[2][2];
	return sum;
}

/***************mobile:�ƶ�Ԫ�أ��ж���һ��***********************
* �涨source��0 1 2 3�ֱ��ʾ�������� 
*/
void mobile(int num)
{
	int temp;
	//�հ�����λ�� 
	int loc; 
	int up = 1,down = 1,left = 1,right = 1;
	loc = loction(num);
	// ��¼��Դ 
	int stand = st[num].source;
	 
	// ��0��ǰһ��״̬����ƶ����հ�λ�� 
	if (loc % 3 != 2 && stand != 2)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3][loc % 3 + 1];
		st[count].status[loc / 3][loc % 3 + 1] = temp;
		st[count].source = 0;
		count++;
	}
 	
 	//��1���ϱ��ƶ����հ�λ��
	if (loc / 3 != 2 && stand != 3)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3 + 1][loc % 3];
		st[count].status[loc / 3 + 1][loc % 3] = temp;
		st[count].source = 1;
		count++;
	}
	
	//��2���ұ��ƶ����հ�λ��
	if (loc % 3 != 0 && stand != 0)
	{
		st[count] = st[num];
		temp = st[count].status[loc / 3][loc % 3];
		st[count].status[loc / 3][loc % 3] = st[count].status[loc / 3][loc % 3 - 1];
		st[count].status[loc / 3][loc % 3 - 1] = temp;
		st[count].source = 2;
		count++;
	}
	
	//��3)�±��ƶ����հ�λ�� 
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
// ***************display���г�ÿһ��״̬*******************
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

//****************search: ����״̬����Ŀ��״̬�Ա� *****
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
			printf("���������޴���\n");
			return 0;
		}
		if (sign(i) == sign(101))
		{
			printf("�ڵ�%d���ҵ���\n", i);
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