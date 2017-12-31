#include<math.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<NTL/ZZ.h>


NTL_CLIENT
using namespace std;


char file1[30]="����.txt";
char file2[30]="����.txt";
char file3[30]="����.txt";	///������: �����ļ���, �����ļ���, ���ܽ���ļ���


/********************************************************************************
*		function: void void Selection( ZZ & e, ZZ euler )						*
*			e:	   ������ִ����ɺ�ȷ���Ĳ���									*
*			ueler: ŷ������ֵ, ����ʵ��ȷ��										*
*		������� e , ֱ��������������Ϊֹ:  e<euler								*
*	����ѧ��, �� e Ϊ������Կ.													*
********************************************************************************/
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



/********************************************************************************
*		function:  void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)			*
*			e: �����ɵļ�����Կ													*
*			d: �����ɵĽ�����Կ													*
*			n: ŷ������ֵ euler(n)												*
*		���ɼ�����Կ e �ͽ�����Կ d ��Ҫ����������������d*e=1 Mod euler(n) ��	*
*	gcd(e,euler)=1 �� e<euler �� d<euler										*
********************************************************************************/
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



/********************************************************************************
*		function:  void SaveKey(const ZZ e, const ZZ d, const ZZ n)				*
*			e: ������Կ															*
*			d ������Կ															*
*			n: ŷ������ֵ euler(n)												*
*		��Կ: PU={e,n}		˽Կ: PR={d,n}										*
*	���湫Կ PU �Լ�˽Կ PR ���ı��ļ�:  PublicKey.txt  PrivateKey.txt			*
********************************************************************************/
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



/********************************************************************************
*		function:  void Encryption(const ZZ e, const ZZ n)						*
*			e: ������Կ															*
*			n: ŷ������ֵ euler(n)												*
*	���ı�����ʽ�������ļ�, ���ֽڼ���: C=M^e(Mod n), ���� M ������, C �Ǽ���	*
*�����ÿ�μ���һ���ֽڣ�Ȼ�󽫼��ܽ�����ı��ļ���ʽд���ļ���					*
********************************************************************************/
void Encryption(const ZZ e, const ZZ n)
{
	ZZ M, C;	//M--- 1���ַ���Ӧ������   C--- 1 ���ַ���Ӧ������

	//cout<<"\n  ������Ҫ���ܵ��ļ�����: *.txt��: "<<flush;	cin>>file1;
	//cout<<"  ���ܵĽ����ŵ��ļ�����:*.txt�� : "<<flush;	cin>>file2;
	fstream fin, fout;
	fin.open ( file1, ios::in );
	fout.open( file2, ios::out );
	if( !fin || !fout )
	{
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t���ڼ���, ��ȴ�...   "<<flush;
	while( !fin.eof() )
	{
		M = fin.get();		//��ȡ�����е�һ���ֽ�
		if( -1==M ) break;	//��ֹ EOF ������

		C = PowerMod( M, e, n );	//����
		//cout<<M<<"\t";
		//cout<<C<<endl;

		fout << (fout, C);		// 1 ���ֽڶ�Ӧ������д���ı�
		fout << (fout, ' ');	// ÿһ�ֽ�����֮���ԡ� ����Ϊ�ָ���
	}
	fout << (fout, "00000");	//���������ļ������λ�ã���֤����ʱ�����쳣

	cout<<"[ �������! ]"<<endl;
	fin.close();
	fout.close();
}




/********************************************************************************
*		function:  void Decryption(const ZZ d, const ZZ n)						*
*			d: ������Կ															*
*			n: ŷ������ֵ euler(n)												*
*	���ı�����ʽ�������ļ�, ���ֽڽ���: M=C^e(Mod n), ���� C ������, M �ǽ���	*
*�����ÿ�ν��ܵõ�һ���ֽڵ����ģ�Ȼ�󽫽������ı��ļ���ʽ���浽�ļ���		*
********************************************************************************/
void Decryption(const ZZ d, const ZZ n)
{
	ZZ C, M;

	//cout<<"\n  �Լ����ļ�����, ���ܽ����ŵ��ļ�����:*.txt��: "<<flush;	cin>>file3;
	fstream fin, fout;
	fin.open ( file2, ios::in);
	fout.open( file3, ios::out);
	if( !fin || !fout )
	{
		cout<<"  ���ļ���������"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t���ڽ���, ��ȴ�...   "<<flush;
	while( !fin.eof() )
	{
		//��00000���Ǽ��ܹ����и��ӵ������ļ�β������Ϣ���ڴ˿ɷ�ֹ EOF �������
		fin >> (fin, C);		//����һ�� ZZ ������ (ʵ����������)
		if( C==00000 ) break;

		M = PowerMod(C, d, n);	//����:	M = d^e Mod n
		//cout<<C<<"\t"<<flush;	//�������
		//cout << M << endl;	//���ܺ������

		/************************************************************************
		��ʵ�ϣ�������ܵõ������� M �� ZZ �͵Ĵ��������������������ģ�������µ�
		for ѭ�������ܵõ������� M ת���� ASCII �ַ���Ȼ��д�롰���ܽ���ļ�����
		����ļ��е����ݲ��������� RSA ���ܽ����
		************************************************************************/
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
	}//end while( )

	cout<<"[ �������! ]"<<endl<<flush;
	fin.close();	//�ر������ļ�
	fout.close();	//�رս��ܽ���ļ�
}


//void Process(const long bits , ZZ &num);
//
//// Selection e, such that e<euler(n)
//void Selection(ZZ & e, ZZ euler );	//ѡ�������Կ e
//
//
////////////  �����Ǹ������ľ���ʵ��  /////////////////////////////////////////////
//
///********************************************************************************
//*		function:  long witness(const ZZ& n, const ZZ& x)						*
//*			n: �����ԵĴ�����													*
//*			x: ��Ϊ��Rabin-Miller����������ɵ�								*
//*	�� n �� RAbin-Miller ���Բ���.   �� n ��ͨ�����, �򷵻� 1 , ���򷵻� 0.	*
//********************************************************************************/
//long witness(const ZZ& n, const ZZ& x)
//{// n�Ǵ����ԵĴ�������x��Ϊ��Rabin-Miller����������ɵ�
//	ZZ m, y, z;
//	long j, k;
//
//	if (x == 0) return 0;
//	// compute m, k such that n-1 = 2^k * m, m odd:
//	k = 1;	m = n/2;
//	while (m % 2 == 0) {
//		k++;
//		m /= 2;
//	}
//
//	z = PowerMod(x, m, n); // z = x^m % n
//	if (z == 1) return 0;
//
//	j = 0;
//	do {
//		y = z;
//		z = (y*y) % n;
//		j++;
//	} while (j < k && z != 1);
//
//	return z != 1 || y != n-1;
//}
//
//
///********************************************************************************
//*		function:  long PrimeTest(const ZZ& n, long t)							*
//*			n: ���������Լ��Ĵ�����											*
//*			t: ���Լ������֤�ݵĲ���											*
//*	�� n �����Լ��.   �� n ��ͨ�����, �򷵻� 1 , ���򷵻� 0.			    *
//********************************************************************************/
//long PrimeTest(const ZZ& n, long t)
//{
//	if (n <= 1) return 0;
//
//	// first, perform trial division by primes up to 2000
//
//	PrimeSeq s;  // a class for quickly generating primes in sequence(����������������ϵ�е���)
//	long p;
//
//	p = s.next();  // first prime is always 2 (��һ�����ɵ���������2,����������ʽ��������)
//	while (p && p < 2000) {	//ֻ��2000���ڵ��������������� n �Ƿ�Ϊ����
//		if ((n % p) == 0) return (n == p);	//����0 or 1
//		p = s.next();
//	}
//
//	// second, perform t Miller-Rabin tests
//
//	ZZ x;
//	long i;
//	for (i = 0; i < t; i++) {	// t ��ʵ�δ��ݣ���Rabin-Miller���Ĵ�����
//		x = RandomBnd(n); // random number between 0 and n-1(��������� [0,n-1] )
//		if (witness(n, x)) return 0;
//	}
//	return 1;
//}
//
//
//
///********************************************************************************
//*		function:  void Process( long bits , ZZ & num )							*
//*			bits: �������ı��س���												*
//*			num:  ������������ bits ���صĸ�������							*
//*		������� bits ���ص������ num , Ȼ���֮������Ϊ 10 �����Լ��, ֱ����	*
//*	���� num �Ǹ�������Ϊֹ.													*
//********************************************************************************/
//void Process( long bits , ZZ & num )
//{
//	ZZ j;
//	j = rand() ;
//
//	while(1)
//	{
//		SetSeed( j );		//����
//		RandomLen( num , bits );	//���� bits ���ص������ num
//		if( PrimeTest( num , 10 ) ) break;	//�� num �����Բ���
//		else j++;	//��֤��������Ӳ�ͬ
//	}
//
//}

