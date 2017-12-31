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
char file1[30]="txt�ļ�/�Ự��Կ����.txt";
char file2[30]="txt�ļ�/�Ự��Կ����.txt";

char file3[30]="txt�ļ�/p_text.txt";
char file4[30]="txt�ļ�/c_text.txt";
char file5[30]="txt�ļ�/p1_text.txt";




unsigned char KEY[8];///�Ự��Կ
unsigned char key[8];///����Կ
unsigned char IV[8]; ///��λ�Ĵ���
unsigned char IV1[8];///����
unsigned char L[4];
unsigned char R[4];
unsigned char A[4][4];///�Ѿ���

void session_key_generation()///���������������64bit�Ự��Կ
{
    for(int i=0;i<8;i++) cout<<int((KEY[i]=rand()%256));
}
void IV_generation()///���������������64bit IV
{
    for(int i=0;i<8;i++) IV[i]=rand()%256;
    for(int i=0;i<8;i++) IV1[i]=IV[i];
}
void IV_recover()
{
    for(int i=0;i<8;i++) IV[i]=IV1[i];
}
void mNxN_mul(unsigned char a[4][4],unsigned char b[4][4],unsigned char v[4][4])  /// 8*8 ������ˣ�v <- a*b
{
 unsigned  char c[4][4];
  for(int i=0;i<4;i++)
	  for(int j=0;j<4;j++)
		  for(unsigned char k=c[i][j]=0;k<4;k++)
			  c[i][j] = (c[i][j]+a[i][k]*b[k][j]) % 2;

  for(int i=0;i<4;i++)
	  for(int j=0;j<4;j++) v[i][j] = c[i][j];
}
unsigned char F(unsigned char r,unsigned char k)///�ֺ���
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
   for(int i=0;i<4-1;i++) ///��ʼ���Ѿ���
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
     for(int i=0;i<8;i++) key[i]=KEY[i]^KEY[(i+1)%8];///��������Կ
      for(int i=0;i<4;i++)
   {
       L[i]=IV[i];
       R[i]=IV[i+4];
   }
   for(int j=0;j<8;j++)///feistel����8��
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

    return L[0];///���ظ�8bit
}
void move_byte(unsigned char c)///��λ�Ĵ�����λ
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
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t���ڼ���, ��ȴ�...   "<<flush;
    while( !fin.eof() )
	{
		m = fin.get();
		if( -1==m ) break;	///��ֹ EOF ������

		cn = feistel_encrypto();///����
		c=cn^m;
		fout << (fout,c);		/// 1 ���ֽڶ�Ӧ������д���ı�
		fout << (fout, ' ');	/// ÿһ�ֽ�����֮���ԡ� ����Ϊ�ָ���
		move_byte(cn);///�������
    }
    cout<<"[ �������! ]"<<endl;

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
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t���ڽ���, ��ȴ�...   "<<flush;
    while( !fin.eof() )
	{
		fin>>(fin,c);
		if(fin.eof()) break;
        cn = feistel_encrypto();///����
		m=c^cn;
		move_byte(cn);///�������
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
     cout<<"[ �������! ]"<<endl;
	fin.close();
	fout.close();
}
void Selection( ZZ & e, ZZ euler )
{
	long bits=100;	//ȡ e �ı��س���Ϊ 300 bits
	ZZ	j;
	time_t	t;

	srand(unsigned( time(&t) ));	//�����б�֤ÿ�β������������ͬ
	j = rand() + j;
	SetSeed( j );		//����
	RandomLen( e, bits );	//���� bits ���ص������ e
}
void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)
{
	ZZ x, s, t;
    cout<<"\n\t����������Կ, ��ȴ�...   "<<flush;
	while( 1 )
	{
		// Randomly select e, such that e<euler(n), e �ĳ��ȹ̶�Ϊ 100 bits
		Selection( e, euler );

		if( 1!=GCD(e,euler) ) continue;	//��֤: e �� euler ����
		else { d=InvMod( e, euler ); break;}	//�� e ����ģ d��ͬʱҲ��֤��: d<euler
	}
	cout<<"[ �Ѿ�������Կ! ]"<<endl;

}
void SaveKey(const ZZ e, const ZZ d, const ZZ n)
{
	cout<<"\n\t���ڱ�����Կ, ��ȴ�...   "<<flush;
	fstream fpublic, fprivate;
	fpublic.open ( "PublicKey.txt", ios::out );
	fprivate.open( "PrivateKey.txt", ios::out );
	if( !fpublic || !fprivate )
	{
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}

	//���湫Կ, ˽Կ���ļ�
	fpublic << e ;	fpublic << "\n\n";	fpublic << n ;		//���湫Կ: PU={e,n}
	fprivate<< d ;	fprivate<< "\n\n";	fprivate<< n ;	//����˽Կ: PR={d,n}

	cout<<"[ ��Կ�������! ]"<<endl;
	cout<<"��Կ�������ļ�:   PublicKey.txt  "<<endl;
	cout<<"˽Կ�������ļ�:   PrivateKey.txt "<<endl;
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
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t���ڼ��ܻỰ��Կ, ��ȴ�...   "<<flush;
	for(int i=0;i<8;i++)
	{
		M = KEY[i];
        C=PowerMod( M, e, n );	///����
        fout << (fout, C);		// 1 ���ֽڶ�Ӧ������д���ı�
		fout << (fout, ' ');	// ÿһ�ֽ�����֮���ԡ� ����Ϊ�ָ���
	}
    cout<<"[ �������! ]"<<endl;
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
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t���ڽ��ܻỰ��Կ, ��ȴ�...   "<<flush;
    for(int i=0;i<8;i++)
    {
        fin >> (fin, C);
        M = PowerMod(C, d, n);
        fout << (fout, M);		// 1 ���ֽڶ�Ӧ������д���ı�
		fout << (fout, ' ');	// ÿһ�ֽ�����֮���ԡ� ����Ϊ�ָ���
    }
 cout<<"[ �������! ]"<<endl;

    fin.close();
	fout.close();
}
#endif // RSA_H_INCLUDED
