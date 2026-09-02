#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> A;
vector<int> T;
inline int get_h(int i) 
{
    return 31 - __builtin_clz((unsigned)i);
}
inline int read()
{
	int x = 0,f = 1;char ch = getchar();
	while(ch < '0' || ch > '9') 
	{
		if(ch == '-') f = -1;
		ch = getchar();
	}
	while(ch >= '0' && ch <= '9')
	{
		x = x * 10 + ch - 48;
		ch = getchar();
	}
	return x * f;
}
inline void write(int x)
{
    static char buf[12];
    int p=0;
    if(x<0){putchar('-');x=-x;}
    if(x==0) buf[p++]='0';
    while(x){buf[p++]=x%10+48;x/=10;}
    while(p--) putchar(buf[p]);
}
int main()
{
	int N = read();int M = read();
	int h = get_h(N);
	A.resize(N + 1,vector<int>(h + 1));
	for(int i = 1;i <= N;i++) A[i][0] = read();
	for(int j = 1;j <= h;j++)
	{
		int k = 1 << j;
		int t = j - 1;
		for(int i = 1;i + k - 1 <= N;i++) A[i][j] = max(A[i][j - 1] , A[i + (1 << t)][j - 1]);
	}
	for(int i = 1;i <= M;i++)
	{
		int l = read();
		int r = read();
		int sz = r - l + 1;
		int mh = get_h(sz);
		write(max(A[l][mh] , A[r - (1 << mh) + 1][mh]));
		putchar('\n');
	}
	return 0;
} 
