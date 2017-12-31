#include "rsa.h";


int main()
{

    ///生成一对公私钥
    ZZ  p, q ;  ///两个 BITS 比特的随机概率素数 p , q
    ZZ  n, e, d, euler; /// n=p*q, euler=(p-1)*(q-1), e---加密密钥,  d---解密密钥
    int bits = BITS;        ///要求概率素数为 BITS bits
    RandomPrime( p, bits, 10 );
    RandomPrime( q, bits, 10 );
    n=p*q;  euler=(p-1)*(q-1);  ///计算 n=p*q 及 欧拉函数值 euler(n)=(p-1)*(q-1)
    KeyGeneration(d, e, euler); ///生成密钥: d, e , where d*e=1 Mod euler(n)
    SaveKey(e, d, n);   ///保存公钥及私钥到文件. 注意: PU={e,n}, PR={d,n}

    ///加密会话密钥和明文
    session_key_generation();///生成会话密钥
    IV_generation();///初始化移位寄存器
    OFBencrypto();///OFB加密
    Encryption(e,n);///会话密钥加密

    ///解密
    Decryption(d,n);///会话密钥解密
    IV_recover();///恢复移位寄存器
    OFBdecrypto();///OFB解密

    return 0;
}
