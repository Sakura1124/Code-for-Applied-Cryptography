#include<math.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<NTL/ZZ.h>


NTL_CLIENT
using namespace std;


char file1[30]="明文.txt";
char file2[30]="密文.txt";
char file3[30]="解密.txt";	///依次是: 明文文件名, 密文文件名, 解密结果文件名


/********************************************************************************
*		function: void void Selection( ZZ & e, ZZ euler )						*
*			e:	   本程序执行完成后确定的参数									*
*			ueler: 欧拉函数值, 已由实参确定										*
*		随机生成 e , 直到满足以下条件为止:  e<euler								*
*	密码学中, 称 e 为加密密钥.													*
********************************************************************************/
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



/********************************************************************************
*		function:  void KeyGeneration(ZZ & d, ZZ & e, const ZZ euler)			*
*			e: 待生成的加密密钥													*
*			d: 待生成的解密密钥													*
*			n: 欧拉函数值 euler(n)												*
*		生成加密密钥 e 和解密密钥 d ，要求满足如下条件：d*e=1 Mod euler(n) 且	*
*	gcd(e,euler)=1 且 e<euler 且 d<euler										*
********************************************************************************/
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



/********************************************************************************
*		function:  void SaveKey(const ZZ e, const ZZ d, const ZZ n)				*
*			e: 加密密钥															*
*			d 加密密钥															*
*			n: 欧拉函数值 euler(n)												*
*		公钥: PU={e,n}		私钥: PR={d,n}										*
*	保存公钥 PU 以及私钥 PR 到文本文件:  PublicKey.txt  PrivateKey.txt			*
********************************************************************************/
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



/********************************************************************************
*		function:  void Encryption(const ZZ e, const ZZ n)						*
*			e: 加密密钥															*
*			n: 欧拉函数值 euler(n)												*
*	以文本流方式打开明文文件, 逐字节加密: C=M^e(Mod n), 其中 M 是明文, C 是加密	*
*结果。每次加密一个字节，然后将加密结果以文本文件方式写入文件。					*
********************************************************************************/
void Encryption(const ZZ e, const ZZ n)
{
	ZZ M, C;	//M--- 1个字符对应的明文   C--- 1 个字符对应的密文

	//cout<<"\n  输入你要加密的文件（如: *.txt）: "<<flush;	cin>>file1;
	//cout<<"  加密的结果存放到文件（如:*.txt） : "<<flush;	cin>>file2;
	fstream fin, fout;
	fin.open ( file1, ios::in );
	fout.open( file2, ios::out );
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t正在加密, 请等待...   "<<flush;
	while( !fin.eof() )
	{
		M = fin.get();		//读取明文中的一个字节
		if( -1==M ) break;	//防止 EOF 被加密

		C = PowerMod( M, e, n );	//加密
		//cout<<M<<"\t";
		//cout<<C<<endl;

		fout << (fout, C);		// 1 个字节对应的密文写入文本
		fout << (fout, ' ');	// 每一字节密文之间以“ ”作为分隔符
	}
	fout << (fout, "00000");	//加在密文文件的最后位置，保证解密时不出异常

	cout<<"[ 加密完成! ]"<<endl;
	fin.close();
	fout.close();
}




/********************************************************************************
*		function:  void Decryption(const ZZ d, const ZZ n)						*
*			d: 解密密钥															*
*			n: 欧拉函数值 euler(n)												*
*	以文本流方式打开密文文件, 逐字节解密: M=C^e(Mod n), 其中 C 是密文, M 是解密	*
*结果。每次解密得到一个字节的明文，然后将解结果以文本文件方式保存到文件。		*
********************************************************************************/
void Decryption(const ZZ d, const ZZ n)
{
	ZZ C, M;

	//cout<<"\n  对加密文件解密, 解密结果存放到文件（如:*.txt）: "<<flush;	cin>>file3;
	fstream fin, fout;
	fin.open ( file2, ios::in);
	fout.open( file3, ios::out);
	if( !fin || !fout )
	{
		cout<<"  打开文件出错！！！"<<endl;
		system("pause");	exit(0);
	}

	cout<<"\n\t正在解密, 请等待...   "<<flush;
	while( !fin.eof() )
	{
		//“00000”是加密过程中附加到密文文件尾部的信息，在此可防止 EOF 被误解密
		fin >> (fin, C);		//读入一个 ZZ 型整数 (实质上是密文)
		if( C==00000 ) break;

		M = PowerMod(C, d, n);	//解密:	M = d^e Mod n
		//cout<<C<<"\t"<<flush;	//输出密文
		//cout << M << endl;	//解密后的明文

		/************************************************************************
		事实上，上面解密得到的明文 M 是 ZZ 型的大整数，并非真正的明文，因此以下的
		for 循环将解密得到的明文 M 转换成 ASCII 字符，然后写入“解密结果文件”。
		最后，文件中的内容才是真正的 RSA 解密结果。
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

	cout<<"[ 解密完成! ]"<<endl<<flush;
	fin.close();	//关闭密文文件
	fout.close();	//关闭解密结果文件
}


//void Process(const long bits , ZZ &num);
//
//// Selection e, such that e<euler(n)
//void Selection(ZZ & e, ZZ euler );	//选择加密密钥 e
//
//
////////////  以下是各函数的具体实现  /////////////////////////////////////////////
//
///********************************************************************************
//*		function:  long witness(const ZZ& n, const ZZ& x)						*
//*			n: 待测试的大整数													*
//*			x: 是为作Rabin-Miller检测而随机生成的								*
//*	对 n 作 RAbin-Miller 素性测试.   若 n 能通过检测, 则返回 1 , 否则返回 0.	*
//********************************************************************************/
//long witness(const ZZ& n, const ZZ& x)
//{// n是待测试的大整数，x是为作Rabin-Miller检测而随机生成的
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
//*			n: 待进行素性检测的大整数											*
//*			t: 素性检测中找证据的参数											*
//*	对 n 作素性检测.   若 n 能通过检测, 则返回 1 , 否则返回 0.			    *
//********************************************************************************/
//long PrimeTest(const ZZ& n, long t)
//{
//	if (n <= 1) return 0;
//
//	// first, perform trial division by primes up to 2000
//
//	PrimeSeq s;  // a class for quickly generating primes in sequence(快速生成有序素数系列的类)
//	long p;
//
//	p = s.next();  // first prime is always 2 (第一次生成的素数总是2,往后以升序方式生成素数)
//	while (p && p < 2000) {	//只用2000以内的素数来检测大整数 n 是否为素数
//		if ((n % p) == 0) return (n == p);	//返回0 or 1
//		p = s.next();
//	}
//
//	// second, perform t Miller-Rabin tests
//
//	ZZ x;
//	long i;
//	for (i = 0; i < t; i++) {	// t 由实参传递（作Rabin-Miller检测的次数）
//		x = RandomBnd(n); // random number between 0 and n-1(生成随机数 [0,n-1] )
//		if (witness(n, x)) return 0;
//	}
//	return 1;
//}
//
//
//
///********************************************************************************
//*		function:  void Process( long bits , ZZ & num )							*
//*			bits: 大整数的比特长度												*
//*			num:  存放随机产生的 bits 比特的概率素数							*
//*		随机生成 bits 比特的随机数 num , 然后对之作参数为 10 的素性检测, 直到产	*
//*	生的 num 是概率素数为止.													*
//********************************************************************************/
//void Process( long bits , ZZ & num )
//{
//	ZZ j;
//	j = rand() ;
//
//	while(1)
//	{
//		SetSeed( j );		//种子
//		RandomLen( num , bits );	//生成 bits 比特的随机数 num
//		if( PrimeTest( num , 10 ) ) break;	//对 num 作素性测试
//		else j++;	//保证随机数种子不同
//	}
//
//}

