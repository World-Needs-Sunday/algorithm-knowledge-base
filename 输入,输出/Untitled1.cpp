#include<bits/stdc++.h> 
using namespace std;
inline int read()
{
	int x=0,f=1;char ch=getchar();
	while (ch<'0'||ch>'9'){if (ch=='-') f=-1;ch=getchar();}
	while (ch>='0'&&ch<='9'){x=x*10+ch-48;ch=getchar();}
	return x*f;
}
inline void write(long long x)
{
    static char buf[30];
    int p=0;
    if(x<0){putchar('-');x=-x;}
    if(x==0) buf[p++]='0';
    while(x){buf[p++]=x%10+48;x/=10;}
    while(p--) putchar(buf[p]);
}
