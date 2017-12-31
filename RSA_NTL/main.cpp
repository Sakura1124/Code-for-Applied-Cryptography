/***********************************************************************

  RSA 加密与解密
  １、p , q  均是 " << bits <<" bits 的大素数(概率素数)；
  ２、加密密钥 e 是 100 bits 的大整数；
  ３、明文文件, 密文文件及解密文件均存放于当前目录下；

***********************************************************************/

#include "head.h"


#define BITS    256     ///要求概率素数 p , q 的比特位数达 BITS bits

int main()
{
    ZZ  p, q ;  ///两个 BITS 比特的随机概率素数 p , q
    ZZ  n, e, d, euler; /// n=p*q, euler=(p-1)*(q-1), e---加密密钥,  d---解密密钥
    int bits = BITS;        ///要求概率素数为 BITS bits

//    cout<<endl;
//    cout<<"**********************************************************"<<endl;
//    cout<<"                 NTL 库实现 RSA 加密与解密             "<<endl;
//    cout<<"  １、p , q  均是 " << bits <<" bits 的大素数(概率素数)； "<<endl;
//    cout<<"  ２、加密密钥 e 是 100 bits 的大整数；                   "<<endl;
//    cout<<"  ３、明文文件, 密文文件及解密文件均存放于当前目录下；    "<<endl;
//    cout<<"**********************************************************"<<endl;


//    system("cls");
    RandomPrime( p, bits, 10 ); RandomPrime( q, bits, 10 );///ZZ.h
    n=p*q;  euler=(p-1)*(q-1);  ///计算 n=p*q 及 欧拉函数值 euler(n)=(p-1)*(q-1)

    KeyGeneration(d, e, euler); ///生成密钥: d, e , where d*e=1 Mod euler(n)
    SaveKey(e, d, n);   ///保存公钥及私钥到文件. 注意: PU={e,n}, PR={d,n}


    //注意: 公钥 KU={e,n}, 用于加密方
    //      私钥 KR={d,n}, 用于解密方

    Encryption(e, n);   //加密: C=M^e(Mod n)
    Decryption(d, n);   //解密: M=C^d(Mod n)

//
//    cout<<endl;
//    cout<<"**********************************************************"<<endl;
//    cout<<"                 NTL 库实现 RSA 加密与解密             "<<endl;
//    cout<<"  １、p , q  均是 " << bits <<" bits 的大素数(概率素数)； "<<endl;
//    cout<<"  ２、加密密钥 e 是 100 bits 的大整数；                   "<<endl;
//    cout<<"  ３、明文文件, 密文文件及解密文件均存放于当前目录下；    "<<endl;
//    cout<<"**********************************************************"<<endl;


    cout<<"\n程序执行完毕!"<<endl;
    cout<<"*********** 以下所有文件均保存于当前目录下 ***********"<<endl;
    cout<<"\t 明文文件:   "<< file1 <<endl;
    cout<<"\t 密文文件:   "<< file2 <<endl;
    cout<<"\t 解密文件:   "<< file3 <<endl;
    cout<<"\t RSA 加密密钥文件:   PublicKey.txt"  << endl;
    cout<<"\t RSA 解密密钥文件:   PrivateKey.txt" << endl;
    cout<<endl;
    cout<<"******************************************************"<<endl;

    cout<<endl;   system("pause");
}
