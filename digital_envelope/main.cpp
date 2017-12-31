#include "rsa.h";


int main()
{

    ///����һ�Թ�˽Կ
    ZZ  p, q ;  ///���� BITS ���ص������������ p , q
    ZZ  n, e, d, euler; /// n=p*q, euler=(p-1)*(q-1), e---������Կ,  d---������Կ
    int bits = BITS;        ///Ҫ���������Ϊ BITS bits
    RandomPrime( p, bits, 10 );
    RandomPrime( q, bits, 10 );
    n=p*q;  euler=(p-1)*(q-1);  ///���� n=p*q �� ŷ������ֵ euler(n)=(p-1)*(q-1)
    KeyGeneration(d, e, euler); ///������Կ: d, e , where d*e=1 Mod euler(n)
    SaveKey(e, d, n);   ///���湫Կ��˽Կ���ļ�. ע��: PU={e,n}, PR={d,n}

    ///���ܻỰ��Կ������
    session_key_generation();///���ɻỰ��Կ
    IV_generation();///��ʼ����λ�Ĵ���
    OFBencrypto();///OFB����
    Encryption(e,n);///�Ự��Կ����

    ///����
    Decryption(d,n);///�Ự��Կ����
    IV_recover();///�ָ���λ�Ĵ���
    OFBdecrypto();///OFB����

    return 0;
}
