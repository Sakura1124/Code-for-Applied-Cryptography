#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include<math.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<NTL/ZZ.h>
#include <iostream>
#include<stdlib.h>


NTL_CLIENT
using namespace std;

#define BITS 256
char file1[30]="txt文件/会话密钥密文.txt";
char file2[30]="txt文件/会话密钥解密.txt";

char file3[30]="txt文件/p_text.txt";
char file4[30]="txt文件/c_text.txt";
char file5[30]="txt文件/p1_text.txt";




unsigned char KEY[8];///会话密钥
unsigned char key[8];///轮密钥
unsigned char IV[8]; ///移位寄存器
unsigned char IV1[8];///备份
unsigned char L[4];
unsigned char R[4];
unsigned char A[4][4];///友矩阵

void session_key_generation()///利用随机函数生成64bit会话密钥
{
    for(int i=0;i<8;i++) cout<<int((KEY[i]=rand()%256));
}
void IV_generation()///利用随机函数生成64bit IV
{
    for(int i=0;i<8;i++) IV[i]=rand()%256;
    for(int i=0;i<8;i++) IV1[i]=IV[i];
}
void IV_recover()
{
    for(int i=0;i<8;i++) IV[i]=IV1[i];
}
void mNxN_mul(unsigned char a[4][4],unsigned char b[4][4],unsigned char v[4][4])  /// 8*8 矩阵相乘，v <- a*b
{
 unsigned  char c[4][4];
  for(int i=0;i<4;i++)
	  for(int j=0;j<4;j++)
		  for(unsigned char k=c[i][j]=0;k<4;k++)
			  c[i][j] = (c[i][j]+a[i][k]*b[k][j]) % 2;

  for(int i=0;i<4;i++)
	  for(int j=0;j<4;j++) v[i][j] = c[i][j];
}
unsigned char F(unsigned char r,unsigned char k)///轮函数
{
   unsigned char n=0;
   int a[4],c[4],t[4];
    for(int i=0;i<4;i++)
    {
        a[i]=r&1;
        r>>=1;
        c[i]=k&1;
        k>>=1;
        t[i]=0;
        A[4][i]=c[i];
    }
   for(int i=0;i<4-1;i++) ///初始化友矩阵
        for(int j=0;j<4;j++)
         if(j==i+1) A[i][j]=1;
         else A[i][j]=0;

    for(int k=0;k<3;k++)
    mNxN_mul(A,A,A);

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++) t[i]=(t[i]+A[i][j]*a[j])%2;

        n+=t[i];
        n<<=1;
    }
     return n%58+65;
}
unsigned char feistel_encrypto()
{
    unsigned char tem;
     for(int i=0;i<8;i++) key[i]=KEY[i]^KEY[(i+1)%8];///生成轮密钥
      for(int i=0;i<4;i++)
   {
       L[i]=IV[i];
       R[i]=IV[i+4];
   }
   for(int j=0;j<8;j++)///feistel加密8轮
   {
        for(int i=0;i<4;i++)
        {
            tem=R[i];
            R[i]=L[i]^F(R[i],key[j]);
            L[i]=tem;
        }
   }
    for(int i=0;i<4;i++)
    {
        tem=L[i];
        L[i]=R[i];
        R[i]=tem;
    }

    return L[0];///返回高8bit
}
void move_byte(unsigned char c)///移位寄存器移位
{
    for(int i=0;i<7;i++)
    {
        IV[i]=IV[i+1];
    }
    IV[7]=c;
}

void OFBencrypto()
{
    int c,cn,m;

    fstream fin,fout;
    fin.open ( file3, ios::in );
	fout.open( file4, ios::out );
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t正在加密, 请等待...   "<<flush;
    while( !fin.eof() )
	{
		m = fin.get();
		if( -1==m ) break;	///防止 EOF 被加密

		cn = feistel_encrypto();///加密
		c=cn^m;
		fout << (fout,c);		/// 1 个字节对应的密文写入文本
		fout << (fout, ' ');	/// 每一字节密文之间以“ ”作为分隔符
		move_byte(cn);///输出反馈
    }
    cout<<"[ 加密完成! ]"<<endl;

	fin.close();
	fout.close();
}
void OFBdecrypto()
{
    int m,cn,c;
    fstream fin,fout;
    fin.open ( file4, ios::in );
	fout.open( file5, ios::out );
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t正在解密, 请等待...   "<<flush;
    while( !fin.eof() )
	{
		fin>>(fin,c);
		if(fin.eof()) break;
        cn = feistel_encrypto();///加密
		m=c^cn;
		move_byte(cn);///输出反馈
      //  fout << (fout, m);
	    char ch;
		for(int i=0;  i<=255; i++)
		 {
			if( 0==(i-m) )
			{
				ch = char(i);
				fout << (fout, ch);
			}
			else continue;
		 }
	}
     cout<<"[ 解密完成! ]"<<endl;
	fin.close();
	fout.close();
}
void Selection( ZZ & e, ZZ euler )
{
	long bits=100;	//取 e 的比特长度为 300 bits
	ZZ	j;
	time_t	t;

	srand(unsigned( time(&t) ));	//这两行保证每次产生的随机数不同
	j = rand() + j;
	SetSeed( j );		//种子
	RandomLen( e, bits );	//生成 bits 比特的随机数 e
}
void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)
{
	ZZ x, s, t;
    cout<<"\n\t正在生成密钥, 请等待...   "<<flush;
	while( 1 )
	{
		// Randomly select e, such that e<euler(n), e 的长度固定为 100 bits
		Selection( e, euler );

		if( 1!=GCD(e,euler) ) continue;	//保证: e 与 euler 互素
		else { d=InvMod( e, euler ); break;}	//求 e 的逆模 d，同时也保证了: d<euler
	}
	cout<<"[ 已经生成密钥! ]"<<endl;

}
void SaveKey(const ZZ e, const ZZ d, const ZZ n)
{
	cout<<"\n\t正在保存密钥, 请等待...   "<<flush;
	fstream fpublic, fprivate;
	fpublic.open ( "PublicKey.txt", ios::out );
	fprivate.open( "PrivateKey.txt", ios::out );
	if( !fpublic || !fprivate )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}

	//保存公钥, 私钥到文件
	fpublic << e ;	fpublic << "\n\n";	fpublic << n ;		//保存公钥: PU={e,n}
	fprivate<< d ;	fprivate<< "\n\n";	fprivate<< n ;	//保存私钥: PR={d,n}

	cout<<"[ 密钥保存完毕! ]"<<endl;
	cout<<"公钥保存在文件:   PublicKey.txt  "<<endl;
	cout<<"私钥保存在文件:   PrivateKey.txt "<<endl;
	fpublic.close();
	fprivate.close();
}
void Encryption(const ZZ e, const ZZ n)
{
    ZZ M, C;
	fstream fout;
	fout.open ( file1, ios::out );
	if(!fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t正在加密会话密钥, 请等待...   "<<flush;
	for(int i=0;i<8;i++)
	{
		M = KEY[i];
        C=PowerMod( M, e, n );	///加密
        fout << (fout, C);		// 1 个字节对应的密文写入文本
		fout << (fout, ' ');	// 每一字节密文之间以“ ”作为分隔符
	}
    cout<<"[ 加密完成! ]"<<endl;
	fout.close();
}
void Decryption(const ZZ d, const ZZ n)
{
   ZZ C, M;
   fstream fin, fout;
   fin.open ( file1, ios::in);
   fout.open( file2, ios::out);
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t正在解密会话密钥, 请等待...   "<<flush;
    for(int i=0;i<8;i++)
    {
        fin >> (fin, C);
        M = PowerMod(C, d, n);
        fout << (fout, M);		// 1 个字节对应的密文写入文本
		fout << (fout, ' ');	// 每一字节密文之间以“ ”作为分隔符
    }
 cout<<"[ 解密完成! ]"<<endl;

    fin.close();
	fout.close();
}
#endif // RSA_H_INCLUDED
