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
	long bits=100;	///ȡ e �ı��س���Ϊ 300 bits
	ZZ	j;
	time_t	t;

	srand(unsigned( time(&t) ));	///�����б�֤ÿ�β������������ͬ
	j = rand() + j;
	SetSeed( j );		///����
	RandomLen( e, bits );	///���� bits ���ص������ e
}

void rgeneration(ZZ & r,ZZ n)
{
    SetSeed(to_ZZ(time(NULL)));
    cout<<"\n\t��������r, ��ȴ�...   "<<flush;
    while(1)
    {
         RandomLen(r,128);///�������128bit��r
         if(1==GCD(r,n)) break;
    }
    cout<<"[ �Ѿ�����r! ]"<<endl;
}


void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)
{
	ZZ x, s, t;

	cout<<"\n\t����������Կ, ��ȴ�...   "<<flush;
	while( 1 )
	{
		// Randomly select e, such that e<euler(n), e �ĳ��ȹ̶�Ϊ 100 bits
		Selection( e, euler );
		//NTL ��: void XGCD(ZZ& d, ZZ& s, ZZ& t, const ZZ& a, const ZZ& b);
		// d = gcd(a, b) = a*s + b*t.
		// XGCD(x, s, t, e, euler);

		if( 1!=GCD(e,euler) ) continue;	//��֤: e �� euler ����

		//if( 1!=x ) continue;	// gcd(e,euler)!=1 , ������ѡ�� e
		else { d=InvMod( e, euler ); break;}	//�� e ����ģ d��ͬʱҲ��֤��: d<euler
	}//end while

//	d = (s>0  ?   s : euler+s);		//��ʱ�� d ����: d*e = 1 Mod euler(n)
	cout<<"[ �Ѿ�������Կ! ]"<<endl;
	//system("cls");
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

void BlindSignature(const ZZ d, const ZZ n,const ZZ re,const ZZ invr)
{
	ZZ S_, S,M,M_;
	fstream fin, fout;
	fin.open ( file1, ios::in);
	fout.open( file2, ios::out);
	if( !fin || !fout )
	{
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t����ǩ��, ��ȴ�...   "<<flush;
	while( !fin.eof() )
	{
		M=fin.get();
		if(-1==M) break;
		M_=(re*M)%n;
        S_= PowerMod(M_, d, n);	///äǩ��
        S=(invr*S_)%n;
        fout << (fout, S);
        fout << (fout, ' ');
	}
	fout << (fout, "00000");
	cout<<"[ǩ�����! ]"<<endl<<flush;
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
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}
	cout<<"\n\t������֤, ��ȴ�...   "<<flush;
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

	cout<<"[��֤���! ]"<<endl<<flush;
	fin.close();
	fout.close();
}

