


#include <time.h>
#include<stdlib.h>
#include<math.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <sys/timeb.h>

#define N 8
#define out(x) printf("%d\n",x)
char FORMAT[]=" %3u";   ///���������ʽ
struct _timeb Time_1,Time_2;
unsigned char  key[16];///����Կ128bit
unsigned char m[16];///����128bit
unsigned char A[8][8];///�Ѿ���


///*********************************************��������**********************************************///
void Seed() /// �õ�ǰʱ������α�����������������
{  unsigned seed;

  _ftime(&Time_1);
  seed=Time_1.time^Time_1.millitm;
  srand(seed);
}
void print(m)///��ӡN*N����
unsigned char m[N][N];
{
    for (int i=0;i<N;i++)
    {
         for(int j=0;j<N;j++)
       {
        printf(FORMAT,m[i][j]);
       }
         putchar('\n');
    }

    putchar('\n');
}
void mNxN_mul(a,b,v)  /// 8*8 ������ˣ�v <- a*b
unsigned char a[8][8],b[8][8],v[8][8];
{
 unsigned  char c[8][8];
  for(int i=0;i<8;i++)
	  for(int j=0;j<8;j++)
		  for(unsigned char k=c[i][j]=0;k<8;k++)
			  c[i][j] = (c[i][j]+a[i][k]*b[k][j]) % 2;

  for(int i=0;i<8;i++)
	  for(int j=0;j<8;j++) v[i][j] = c[i][j];
}

unsigned char F(unsigned char r,unsigned char k)///�ֺ���
{
   unsigned char n=0;
   int a[8],c[8],t[8];
    for(int i=0;i<8;i++)
    {
        a[i]=r&1;
        r>>=1;
        c[i]=k&1;
        k>>=1;
        t[i]=0;
        A[7][i]=c[i];
    }
   for(int i=0;i<8-1;i++) ///��ʼ���Ѿ���
        for(int j=0;j<8;j++)
         if(j==i+1) A[i][j]=1;
         else A[i][j]=0;

    for(int k=0;k<3;k++)
        mNxN_mul(A,A,A);

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++) t[i]=(t[i]+A[i][j]*a[j])%2;

        n+=t[i];
        n<<=1;
    }
     return n%58+65;
}

///******************************************������******************************************///
int main()
{
   unsigned char L[8],R[8],t[8],tem;
   Seed();
   printf("�����������Կ\n");
   for(int i=0;i<16;i++) printf("%d ",key[i]=rand()%256);///�����������Կ
    printf("\n");
     printf("����Կ\n");
   for(int i=0;i<16;i++) printf("%d ",key[i]=key[i]^key[(i+1)%16]);///����Կ
   printf("\n");
   printf("������16����ĸ\n");
   gets(m);
   if(strlen(m)<16)
    for(int i=strlen(m);i<16;i++)
       m[i]='a';

   for(int i=0;i<8;i++)
   {
       L[i]=m[i];
       R[i]=m[i+8];
   }
    for(int i=0;i<8;i++)printf("%c",L[i]);
    for(int i=0;i<8;i++)printf("%c",R[i]);
    printf("\n");

   for(int j=0;j<1;j++)
   {
        for(int i=0;i<8;i++)
        {
            tem=R[i];
            R[i]=L[i]^F(R[i],key[j]);
            L[i]=tem;
        }
   }
    for(int i=0;i<8;i++)
    {
        tem=L[i];
        L[i]=R[i];
        R[i]=tem;
    }
    printf("����Ϊ��\n");
    for(int i=0;i<8;i++)printf("%c",L[i]);
    for(int i=0;i<8;i++)printf("%c",R[i]);
    printf("\n");
    for(int i=0;i<16;i++) printf("%d ",key[i]);///����Կ


   for(int j=0;j<1;j++)
   {
        for(int i=0;i<8;i++)
        {
            tem=R[i];
            R[i]=L[i]^F(R[i],key[0-j]);
            L[i]=tem;
        }
   }
    for(int i=0;i<8;i++)
    {
        tem=L[i];
        L[i]=R[i];
        R[i]=tem;
    }
    printf("����Ϊ��\n");
    for(int i=0;i<8;i++)printf("%c",L[i]);
    for(int i=0;i<8;i++)printf("%c",R[i]);
    printf("\n");
}
