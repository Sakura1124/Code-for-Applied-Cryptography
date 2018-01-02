/*
   第二次作业：构造Fq上的n级LFSR，使其所生成的任一非零序列均为m序列
*/
#include <time.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>

#define out(x) printf("%d\n",x)
#define N 8  ///N为LFSR级数
#define NN (N*N)
#define Q 2  ///Q为素数
char FORMAT[]=" %3u";   ///设置输出格式
FILE *fp;         ///输出文件
unsigned A[N][N];  ///友矩阵
unsigned M[N][1];  ///初始序列
unsigned I[N][N];///单位矩阵
struct _timeb Time_1,Time_2;
unsigned Cur_Sec,Cur_Millisec,Totle_Sec,Totle_Millisec;

///----------------------------------------------其他函数-------------------------------------------------------------------------------

void Seed() /// 用当前时间设置伪随机数发生器的种子
{ unsigned seed;

  _ftime(&Time_1);
  seed=Time_1.time^Time_1.millitm;
  srand(seed);
}
void print(m)///打印N*N矩阵
unsigned m[N][N];
{
    for (int i=0;i<N;i++)
    {
         for(int j=0;j<N;j++)
       {
        printf(FORMAT,m[i][j]);
        fprintf(fp,FORMAT,m[i][j]);
       }
         putchar('\n');
         putc('\n',fp);
    }

    putchar('\n');
    putc('\n',fp);
}
void init()        ///初始化友矩阵前N-1行
{
   for(int i=0;i<N-1;i++)
        for(int j=0;j<N;j++)
         if(j==i+1)
            A[i][j]=1;
         else
            A[i][j]==0;
  }
int mNxN_eq(m,M) /// 如果 N*N 矩阵 m=M，返回1，否则返回0
unsigned m[N][N],M[N][N];
{
  for(int i=0;i<N;i++)
    for(int j=0;j<N;j++)
	 if(m[i][j] != M[i][j]) return 0;
  return 1;
}
void mNxN_copy(m,M) /// N*N 矩阵拷贝，m -> M
unsigned m[NN],M[NN]; /// 采用一维表示，可减少计算坐标开销
{ unsigned i;

  for(i=0;i<NN;i++)
	  M[i]=m[i];
}
void mNxN_mul(a,b,v)  /// N*N 矩阵相乘，v <- a*b
unsigned  a[N][N],b[N][N],v[N][N];
{
  unsigned  c[N][N];
  for(int i=0;i<N;i++)
	  for(int j=0;j<N;j++)
		  for(int k=c[i][j]=0;k<N;k++)
			  c[i][j] = (c[i][j]+a[i][k]*b[k][j]) % Q;

  for(int i=0;i<N;i++)
	  for(int j=0;j<N;j++) v[i][j] = c[i][j];
}
int mNxN_time(m)///求友矩阵周期
unsigned m[N][N];
{
    int num=1;
    unsigned temp[N][N];
    mNxN_copy(m,temp);
   if(temp[N-1][0]==0)
    {
        return -1;
    }
    while(!mNxN_eq(temp,I))
    {
       mNxN_mul(temp,m,temp);
          num++;

    }
    return num;
}
///----------------------------------------------主函数-----------------------------------------------------------------------------------
int main()
{


        if((fp=fopen(".\\LFSR_m.txt","a"))==NULL)
     {
	  puts("Can create file: LFSR_m.txt");
	  exit(0);
     }
       for(int i=0;i<N;i++)   ///初始化单位阵
         for(int j=0;j<N;j++)
           if(i==j)
           I[i][j]=1;
       printf("F%d上的%d级LFSR生成的m序列：\n\n",Q,N);
       fprintf(fp,"F%d上的%d级LFSR生成的m序列：\n\n",Q,N);
       init();///初始化友矩阵
    Retry:
       Seed();
        for(int j=0;j<N;j++)
        {
            A[N-1][j]=rand()%Q;

        }
       while(mNxN_time(A)!=pow(Q,N)-1)///随机寻找符合条件的一个友矩阵
      {
         for(int j=0;j<N;j++)
        {
            A[N-1][j]=rand()%Q;

        }

      }
       printf("满足条件的一个友矩阵:\n");
       fprintf(fp,"满足条件的一个友矩阵:\n");
       print(A);
       printf("\n按 Esc 结束，其它键继续 ...\n");
       if(getch()!= 27) goto Retry;

}
