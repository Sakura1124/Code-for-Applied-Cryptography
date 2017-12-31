#include <iostream>
#include"rsa.h"


using namespace std;

#define BITS 256

void test()
{
    ZZ re=to_ZZ("468468487864641454");
    ZZ m=to_ZZ("45645645641454");
    cout<<m*re;

}
int main()
{
//    test();
//    return 0;
    ZZ  p, q ;  ///两个 BITS 比特的随机概率素数 p , q
    ZZ  n, e, d, euler; /// n=p*q, euler=(p-1)*(q-1), e---公钥,  d---私钥
    int bits = BITS;        ///要求概率素数为 BITS bits
    RandomPrime( p, bits, 10 ); RandomPrime( q, bits, 10 );///ZZ.h
    n=p*q;  euler=(p-1)*(q-1);  ///计算 n=p*q 及 欧拉函数值 euler(n)=(p-1)*(q-1)

    KeyGeneration(d, e, euler); ///生成密钥: d, e , where d*e=1 Mod euler(n)
    SaveKey(e, d, n);   ///保存公钥及私钥到文件. 注意: PU={e,n}, PR={d,n}

    ZZ re,r,invr;
    rgeneration(r,n);

    re=PowerMod(r, e, n);
    invr=InvMod(r,n);///求r mod n 的逆元

    BlindSignature(d, n,re,invr); ///签名
    verify(e,n);

}
