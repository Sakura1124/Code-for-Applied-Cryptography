#include<math.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<NTL/ZZ.h>


NTL_CLIENT
using namespace std;


char file1[30]="m_text.txt";
char file2[30]="s_text.txt";
char file3[30]="v_text.txt";

void Selection( ZZ & e, ZZ euler )
{
	long bits=100;	///取 e 的比特长度为 300 bits
	ZZ	j;
	time_t	t;

	srand(unsigned( time(&t) ));	///这两行保证每次产生的随机数不同
	j = rand() + j;
	SetSeed( j );		///种子
	RandomLen( e, bits );	///生成 bits 比特的随机数 e
}

void rgeneration(ZZ & r,ZZ n)
{
    SetSeed(to_ZZ(time(NULL)));
    cout<<"\n\t正在生成r, 请等待...   "<<flush;
    while(1)
    {
         RandomLen(r,128);///随机生成128bit的r
         if(1==GCD(r,n)) break;
    }
    cout<<"[ 已经生成r! ]"<<endl;
}


void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)
{
	ZZ x, s, t;

	cout<<"\n\t正在生成密钥, 请等待...   "<<flush;
	while( 1 )
	{
		// Randomly select e, such that e<euler(n), e 的长度固定为 100 bits
		Selection( e, euler );
		//NTL 库: void XGCD(ZZ& d, ZZ& s, ZZ& t, const ZZ& a, const ZZ& b);
		// d = gcd(a, b) = a*s + b*t.
		// XGCD(x, s, t, e, euler);

		if( 1!=GCD(e,euler) ) continue;	//保证: e 与 euler 互素

		//if( 1!=x ) continue;	// gcd(e,euler)!=1 , 则重新选择 e
		else { d=InvMod( e, euler ); break;}	//求 e 的逆模 d，同时也保证了: d<euler
	}//end while

//	d = (s>0  ?   s : euler+s);		//此时的 d 满足: d*e = 1 Mod euler(n)
	cout<<"[ 已经生成密钥! ]"<<endl;
	//system("cls");
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

void BlindSignature(const ZZ d, const ZZ n,const ZZ re,const ZZ invr)
{
	ZZ S_, S,M,M_;
	fstream fin, fout;
	fin.open ( file1, ios::in);
	fout.open( file2, ios::out);
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t正在签名, 请等待...   "<<flush;
	while( !fin.eof() )
	{
		M=fin.get();
		if(-1==M) break;
		M_=(re*M)%n;
        S_= PowerMod(M_, d, n);	///盲签名
        S=(invr*S_)%n;
        fout << (fout, S);
        fout << (fout, ' ');
	}
	fout << (fout, "00000");
	cout<<"[签名完成! ]"<<endl<<flush;
	fin.close();
	fout.close();
}
void verify(const ZZ e,const ZZ n)
{
    ZZ S,M;
    fstream fin, fout;
    fin.open ( file2, ios::in);
	fout.open( file3, ios::out);
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t正在验证, 请等待...   "<<flush;
    while( !fin.eof() )
	{
		fin >> (fin, S);
		if( S==00000 ) break;

		M = PowerMod(S, e, n);

		char ch;
		for(int i=0;  i<=255; i++)
		{
			if( 0==(i-M) )
			{
				ch = char(i);
				fout << (fout, ch);
			}
			else continue;
		}//end for
		cout<<flush;
	}

	cout<<"[验证完成! ]"<<endl<<flush;
	fin.close();
	fout.close();
}

