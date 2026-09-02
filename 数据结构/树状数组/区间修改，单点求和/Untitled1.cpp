#include<bits/stdc++.h>
/*洛谷P3368*/
using namespace std; 
vector<long long> treearr;
inline int lowbit(int i)
{
	return i & -i;
}

void init(int n)
{
	treearr.resize(n + 1,0);
	long long A = 0;
	for(int i = 1;i <= n;i++)
	{
		int x,w;
		cin >> x;
		w = x - A;
		A = x;
		
		int idx = i;
		while(idx <= n)
		{
			treearr[idx] += w;
			idx += lowbit(idx);
		}
	}
	
	 
}

void add(int i,int k,int n)
{
	while(i <= n)
	{
		treearr[i] += k;
		i += lowbit(i);
	}
}

void add(int l,int r,int k,int n)
{
	add(l,k,n);
	add(r + 1,-k,n);
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
		int op;
		cin >> op;
		if(op == 1)
		{
			int x,y,k;
			cin >> x >> y >> k;
			add(x,y,k,n);
		}
		else
		{
			int x;
			cin >> x; 
			cout << get(x) << '\n';
		}
	}
	return 0;
}
