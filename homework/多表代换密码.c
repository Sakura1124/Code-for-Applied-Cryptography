/*

         ��һ����ҵ����m �ϵĶ���������
         N Ϊ����ά��
         MOD Ϊ Zm ��ģ��
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 5
#define MOD 26

FILE *fp;///����ļ�

int A[N][N];
int A1[N][N];///A�İ������
int B[N][N];
int B1[N][N];///B�İ������
int M[N][N];///���ľ���
int T[N][N];
int C[N][N];///���ľ���
int T1[N][N];
int T2[N][N];
int inv(int n)///����Ԫ
{
   for(int i=1;i<27;i++)
        if(i*n%MOD==1)
          return i;
}
void add(int a[N][N],int b[N][N])///�������
{
    for(int i=0;i<N;i++)
  {

      for(int j=0;j<N;j++)
      {
        C[i][j]=(a[i][j]+b[i][j])%MOD;
      }
  }

}
void sub(int a[N][N],int b[N][N])///�������
{
    for(int i=0;i<N;i++)
  {

      for(int j=0;j<N;j++)
      {
        C[i][j]=(a[i][j]-b[i][j]+26)%MOD;
      }
  }

}
void mul(int a[N][N],int b[N][N],int c[N][N])///�������
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
int det(int arcs[N][N],int n)///����һ��չ����������ʽ
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

void adj(int arcs[N][N],int n,int ans[N][N])///����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
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

int gcd(int a,int b)///���������
{
    return b?gcd(b,a%b):a;
}

int main()    ///������
{

  char s[50];
  srand((unsigned)time(NULL));
  if((fp=fopen(".\\MATRIX_Zm.txt","a"))==NULL)
  {
	  puts("Can create file: MATRIX_Zm.txt.txt");
	  exit(0);
  }
 while(det(A,N)==0||gcd(det(A,N),MOD)!=1)///����ԿA
{
      for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
        A[i][j]=rand()%26;

  }

}
while(det(B,N)==0||gcd(det(B,N),MOD)!=1)///����ԿB
{
    for(int i=0;i<N;i++)
  {
      for(int j=0;j<N;j++)
        B[i][j]=rand()%26;

  }
}
 for(int i=0;i<N;i++)///����ԿT
  {
      for(int j=0;j<N;j++)
        T[i][j]=rand()%26;

  }
printf("A����:\n");
fprintf(fp,"A����:\n");
 for(int i=0;i<N;i++)///�������
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", A[i][j]);
       fprintf(fp," %02d",A[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
printf("B����:\n");
fprintf(fp,"B����:\n");
   for(int i=0;i<N;i++)///�������
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", B[i][j]);
       fprintf(fp," %02d",B[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
printf("T����:\n");
fprintf(fp,"T����:\n");
 for(int i=0;i<N;i++)///�������
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
  for(int i=0;i<N;i++)///��ȡ����
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
  printf("M����:\n");
  fprintf(fp,"M����:\n");
 for(int i=0;i<N;i++)///�������
  {

      for(int j=0;j<N;j++)
      {
       printf(" %02d", M[i][j]);
       fprintf(fp," %02d",M[i][j]);
      }
      printf("\n");
      putc('\n',fp);
  }
 mul(A,M,T1);///����
 mul(T1,B,C);
 add(C,T);
 printf("���ģ�\n");
 fprintf(fp,"����:\n");
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
  sub(C,T);///����
  adj(A,N,A1);
  adj(B,N,B1);
  mul(A1,C,T2);
  mul(T2,B1,C);
  printf("���ģ�\n");
  fprintf(fp,"����:\n");
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







