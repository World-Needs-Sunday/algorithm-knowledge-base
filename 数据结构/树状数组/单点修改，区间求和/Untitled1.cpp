#include<bits/stdc++.h>
using namespace std;
/*洛谷P3374*/
vector<long long> treearr;
inline int lowbit(int i)
{
	return i & -i;
}

void init(int n)
{
	treearr.resize(n + 1,0);
	for(int i = 1;i <= n;i++)
	{
		int x;
		cin >> x;
		int idx = i;
		while(idx <= n)
		{
			treearr[idx] += x;
			idx += lowbit(idx);
		}
	}
}

void add(int i, int k,int n)
{
	while(i <= n)
	{
		treearr[i] += k;
		i += lowbit(i);
	}
}

long long get(int i)
{
	long long ans = 0;
	while(i)
	{
		ans += treearr[i];
		i -= lowbit(i);
	}
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,m;
	cin >> n >> m;
	init(n);
	for(int i = 0;i < m;i++)
	{
		int op,x,y;
		cin >> op >> x >> y;
		if(op == 1) add(x,y,n);
		else cout << get(y) - get(x - 1) << '\n';
	}
	return 0;	
} 
