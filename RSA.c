#include<stdio.h>
#include<stdlib.h>
#include<time.h>



typedef  long long ll;
union m
{
    char s[64];
    ll num;
};


ll gcd(ll a,ll b)///求最大公因数
{
    return b?gcd(b,a%b):a;
}

ll inverse(ll n, ll modulus) {
    ll a = n, b = modulus;
    ll x = 0, y = 1, x0 = 1, y0 = 0, q, temp;
    while(b != 0) {
        q = a / b;
        temp = a % b;
        a = b;
        b = temp;
        temp = x; x = x0 - q * x; x0 = temp;
        temp = y; y = y0 - q * y; y0 = temp;
    }
    if(x0 < 0) x0 += modulus;
    return x0;
}
ll qpow(ll a, ll b, ll c) {
    ll res = 1;
    while(b > 0) {
        /* Need long multiplication else this will overflow... */
        if(b & 1) {
            res = (res * a) % c;
        }
        b = b >> 1;
        a = (a * a) % c; /* Same deal here */
    }
    return res;
}
int main()
{
   // union m m1,c,m2;
    ll m1,c,m2;
    srand((unsigned)time(NULL));
    ll p=947 ,q=1489 ,r=877,e,d;
    ll n=p*q*r;
    ll fn=(p-1)*(q-1)*(r-1);
    printf("n=%lld\n",n);
    printf("fn=%lld\n",fn);
    do
        {e=(rand()+fn)%fn;}while(gcd(e,fn)!=1);

    printf("随机选取e=%lld\n",e);
    printf("d=%lld\n",d=inverse(e,fn));
  //  printf("%lld\n",e*d%fn);
    printf("请输入明文：\n");
//    gets(m1.m_s);
//    c.m_num=qpow(m1.m_num,e);
//    printf("密文为：\n%s",c.m_s);

//    m2.m_num=qpow(c.m_num,d);
//    printf("解密为：\n%s",m2.m_s);
scanf("%lld",&m1);
c=qpow(m1,e,n);
printf("密文为：%lld\n",c);
m2=qpow(c,d,n);
printf("解密为：%lld\n",m2);



}



