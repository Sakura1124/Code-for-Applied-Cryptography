/*
   �ڶ�����ҵ������Fq�ϵ�n��LFSR��ʹ�������ɵ���һ�������о�Ϊm����
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
#define N 8  ///NΪLFSR����
#define NN (N*N)
#define Q 2  ///QΪ����
char FORMAT[]=" %3u";   ///���������ʽ
FILE *fp;         ///����ļ�
unsigned A[N][N];  ///�Ѿ���
unsigned M[N][1];  ///��ʼ����
unsigned I[N][N];///��λ����
struct _timeb Time_1,Time_2;
unsigned Cur_Sec,Cur_Millisec,Totle_Sec,Totle_Millisec;

///----------------------------------------------��������-------------------------------------------------------------------------------

void Seed() /// �õ�ǰʱ������α�����������������
{ unsigned seed;

  _ftime(&Time_1);
  seed=Time_1.time^Time_1.millitm;
  srand(seed);
}
void print(m)///��ӡN*N����
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
void init()        ///��ʼ���Ѿ���ǰN-1��
{
   for(int i=0;i<N-1;i++)
        for(int j=0;j<N;j++)
         if(j==i+1)
            A[i][j]=1;
         else
            A[i][j]==0;
  }
int mNxN_eq(m,M) /// ��� N*N ���� m=M������1�����򷵻�0
unsigned m[N][N],M[N][N];
{
  for(int i=0;i<N;i++)
    for(int j=0;j<N;j++)
	 if(m[i][j] != M[i][j]) return 0;
  return 1;
}
void mNxN_copy(m,M) /// N*N ���󿽱���m -> M
unsigned m[NN],M[NN]; /// ����һά��ʾ���ɼ��ټ������꿪��
{ unsigned i;

  for(i=0;i<NN;i++)
	  M[i]=m[i];
}
void mNxN_mul(a,b,v)  /// N*N ������ˣ�v <- a*b
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
int mNxN_time(m)///���Ѿ�������
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
///----------------------------------------------������-----------------------------------------------------------------------------------
int main()
{


        if((fp=fopen(".\\LFSR_m.txt","a"))==NULL)
     {
	  puts("Can create file: LFSR_m.txt");
	  exit(0);
     }
       for(int i=0;i<N;i++)   ///��ʼ����λ��
         for(int j=0;j<N;j++)
           if(i==j)
           I[i][j]=1;
       printf("F%d�ϵ�%d��LFSR���ɵ�m���У�\n\n",Q,N);
       fprintf(fp,"F%d�ϵ�%d��LFSR���ɵ�m���У�\n\n",Q,N);
       init();///��ʼ���Ѿ���
    Retry:
       Seed();
        for(int j=0;j<N;j++)
        {
            A[N-1][j]=rand()%Q;

        }
       while(mNxN_time(A)!=pow(Q,N)-1)///���Ѱ�ҷ���������һ���Ѿ���
      {
         for(int j=0;j<N;j++)
        {
            A[N-1][j]=rand()%Q;

        }

      }
       printf("����������һ���Ѿ���:\n");
       fprintf(fp,"����������һ���Ѿ���:\n");
       print(A);
       printf("\n�� Esc ���������������� ...\n");
       if(getch()!= 27) goto Retry;

}
