/*

         第一次作业：Ｚm 上的多表代换密码
         N 为矩阵维数
         MOD 为 Zm 的模数
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 5
#define MOD 26

FILE *fp;///输出文件

int A[N][N];
int A1[N][N];///A的伴随矩阵
int B[N][N];
int B1[N][N];///B的伴随矩阵
int M[N][N];///明文矩阵
int T[N][N];
int C[N][N];///密文矩阵
int T1[N][N];
int T2[N][N];
int inv(int n)///求逆元
{
   for(int i=1;i<27;i++)
        if(i*n%MOD==1)
          return i;
}
void add(int a[N][N],int b[N][N])///矩阵相加
{
    for(int i=0;i<N;i++)
  {

      for(int j=0;j<N;j++)
      {
        C[i][j]=(a[i][j]+b[i][j])%MOD;
      }
  }

}
void sub(int a[N][N],int b[N][N])///矩阵相减
{
    for(int i=0;i<N;i++)
  {

      for(int j=0;j<N;j++)
      {
        C[i][j]=(a[i][j]-b[i][j]+26)%MOD;
      }
  }

}
void mul(int a[N][N],int b[N][N],int c[N][N])///矩阵相乘
{
    for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
      {
           for(int k=0;k<N;k++)
            c[i][j]=(c[i][j]+a[i][k]*b[k][j])%MOD;
      }
  }
}
int det(int arcs[N][N],int n)///按第一行展开计算行列式
{
    if(n==1)
    {
        return arcs[0][0];
    }
    int ans = 0;
    int temp[N][N];
    int i,j,k;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1;j++)
        {
            for(k=0;k<n-1;k++)
            {
                temp[j][k] = arcs[j+1][(k>=i)?k+1:k];

            }
        }
        int t = det(temp,n-1);
        if(i%2==0)
        {
            ans += arcs[0][i]*t;
        }
        else
        {
            ans -=  arcs[0][i]*t;
        }
    }
    return (ans%26+26)%26;
}

void adj(int arcs[N][N],int n,int ans[N][N])///计算每一行每一列的每个元素所对应的余子式，组成A*
{
    if(n==1)
    {
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    int temp[N][N];
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            for(k=0;k<n-1;k++)
            {
                for(t=0;t<n-1;t++)
                {
                    temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
                }
            }


            ans[j][i]  =  det(temp,n-1);
            if((i+j)%2 == 1)
            {
                ans[j][i] = - ans[j][i];
            }
        }
    }
}

int gcd(int a,int b)///求最大公因数
{
    return b?gcd(b,a%b):a;
}

int main()    ///主函数
{

  char s[50];
  srand((unsigned)time(NULL));
  if((fp=fopen(".\\MATRIX_Zm.txt","a"))==NULL)
  {
	  puts("Can create file: MATRIX_Zm.txt.txt");
	  exit(0);
  }
 while(det(A,N)==0||gcd(det(A,N),MOD)!=1)///求密钥A
{
      for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
        A[i][j]=rand()%26;

  }

}
while(det(B,N)==0||gcd(det(B,N),MOD)!=1)///求密钥B
{
    for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
        B[i][j]=rand()%26;

  }
}
 for(int i=0;i<N;i++)///求密钥T
  {
      for(int j=0;j<N;j++)
        T[i][j]=rand()%26;

  }
printf("A矩阵:\n");
fprintf(fp,"A矩阵:\n");
 for(int i=0;i<N;i++)///输出矩阵
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", A[i][j]);
       fprintf(fp," %02d",A[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
printf("B矩阵:\n");
fprintf(fp,"B矩阵:\n");
   for(int i=0;i<N;i++)///输出矩阵
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", B[i][j]);
       fprintf(fp," %02d",B[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
printf("T矩阵:\n");
fprintf(fp,"T矩阵:\n");
 for(int i=0;i<N;i++)///输出矩阵
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", T[i][j]);
       fprintf(fp," %02d",T[i][j]);
      }
       printf("\n");
       putc('\n',fp);
  }
  int num=0;
  scanf("%s",&s);
  for(int i=0;i<N;i++)///读取明文
  {
      for(int j=0;j<N;j++)
        {

            if(s[num]!='\0')
              {
                M[i][j]=s[num]-97;
                num++;
              }
              else
                M[i][j]=0;

        }

  }
  printf("M矩阵:\n");
  fprintf(fp,"M矩阵:\n");
 for(int i=0;i<N;i++)///输出矩阵
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", M[i][j]);
       fprintf(fp," %02d",M[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
 mul(A,M,T1);///加密
 mul(T1,B,C);
 add(C,T);
 printf("密文：\n");
 fprintf(fp,"密文:\n");
  for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
      {
        printf("%c",((C[i][j]+26)%26)+97);
        fprintf(fp," %c",((C[i][j]+26)%26)+97);
      }


  }
  printf("\n");
  putc('\n',fp);
  sub(C,T);///解密
  adj(A,N,A1);
  adj(B,N,B1);
  mul(A1,C,T2);
  mul(T2,B1,C);
  printf("明文：\n");
  fprintf(fp,"明文:\n");
  for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
      {
          C[i][j]=C[i][j]*inv(det(A,N))*inv(det(B,N))%MOD;
          printf("%c",M[i][j]+97);
          fprintf(fp," %c",M[i][j]+97);

      }

  }
  fclose(fp);

}







