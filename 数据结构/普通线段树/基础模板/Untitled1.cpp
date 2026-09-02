#include<bits/stdc++.h>
/*洛谷3372*/ 
using namespace std;
vector<long long> tree;
vector<long long> lazy;
void init(int rt,int l,int r)
{
	if(l == r)
	{
		tree[rt] = l;
		return;
	}
	int mid = (l + r) >> 1;
	init(rt << 1 , l , mid);
	init((rt << 1) + 1, mid + 1,r);
	tree[rt] = tree[rt << 1] + tree[(rt << 1) + 1]; 
}
void init(int n)
{
	lazy.resize(4 * n + 1 , 0);
	tree.resize(n * 4 + 1);
	init(1,1,n);
}
void my_down(int i ,int l,int r)
{
	if(lazy[i] == 0 || l == r) return;
	int mid = (l + r) >> 1;
	lazy[i << 1] += lazy[i];
	tree[i << 1] += (long long)(mid - l + 1) * lazy[i];
	lazy[(i << 1) + 1] += lazy[i];
	tree[(i << 1) + 1] += (long long)(r - mid) * lazy[i];
	lazy[i] = 0;
}
long long my_get(int rt,int l,int r,const int& nl,const int& nr)
{
	my_down(rt,l,r);
	if(l > nr || r < nl) return 0;
	else if(nl <= l && nr >= r) return tree[rt];
	else 
	{
		int mid = (l + r) >> 1;
		return my_get(rt << 1 , l , mid , nl , nr) + my_get((rt << 1) + 1, mid + 1,r,nl,nr);
	}
}
void my_add(int rt,int l,int r,const int& nl,const int& nr,const long long& k)
{
	my_down(rt,l,r);
	if(l > nr || r < nl) return;
	if(nl <= l && nr >= r) 
	{
		tree[rt] += (long long)(r - l + 1) * k;
		lazy[rt] += k;
		return;
	}
	int mid = (l + r) >> 1;
	my_add(rt << 1,l,mid,nl,nr,k);
	my_add((rt << 1) + 1,mid + 1,r,nl,nr,k);
	tree[rt] = tree[rt << 1] + tree[(rt << 1) + 1]; 
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
			int x,y;
			long long k; 
			cin >> x >> y >> k;
			my_add(1,1,n,x,y,k);
		}
		else 
		{
			int x,y;
			cin >> x >> y;
			cout << my_get(1,1,n,x,y) << '\n';
		}
	}
	return 0;
} 
