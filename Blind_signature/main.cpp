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
    ZZ  p, q ;  ///���� BITS ���ص������������ p , q
    ZZ  n, e, d, euler; /// n=p*q, euler=(p-1)*(q-1), e---��Կ,  d---˽Կ
    int bits = BITS;        ///Ҫ���������Ϊ BITS bits
    RandomPrime( p, bits, 10 ); RandomPrime( q, bits, 10 );///ZZ.h
    n=p*q;  euler=(p-1)*(q-1);  ///���� n=p*q �� ŷ������ֵ euler(n)=(p-1)*(q-1)

    KeyGeneration(d, e, euler); ///������Կ: d, e , where d*e=1 Mod euler(n)
    SaveKey(e, d, n);   ///���湫Կ��˽Կ���ļ�. ע��: PU={e,n}, PR={d,n}

    ZZ re,r,invr;
    rgeneration(r,n);

    re=PowerMod(r, e, n);
    invr=InvMod(r,n);///��r mod n ����Ԫ

    BlindSignature(d, n,re,invr); ///ǩ��
    verify(e,n);

}
