#include<bits/stdc++.h> 
using namespace std;
vector<long long> tree;
vector<long long> mul_lazy;
vector<long long> sum_lazy;
void init(int rt,int l,int r,int m,const vector<long long>& A)
{
	if(l == r)
	{
		tree[rt] = A[l];
		return;
	}
	int mid = (l + r) >> 1;
	init(rt << 1,l,mid,m,A);
	init((rt << 1) + 1,mid + 1,r,m,A);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) + 1]) % m;
}
void init(int n,int m)
{
	vector<long long> A(n + 1);
	tree.resize(4 * n + 1);
	mul_lazy.resize(4 * n + 1 , 1);
	sum_lazy.resize(4 * n + 1 , 0);
	for(int i = 1;i <= n;i++)
	{
		cin >> A[i];
		A[i] %= m;
	}
	init(1,1,n,m,A);
}
void my_down(int rt,int l,int r,int mid,int m)
{
	if(l == r) return;
	if(mul_lazy[rt] != 1)
	{
		mul_lazy[rt << 1] = (mul_lazy[rt] * mul_lazy[rt << 1]) % m;
		tree[rt << 1] = (tree[rt << 1] * mul_lazy[rt]) % m;
		sum_lazy[rt << 1] = (sum_lazy[rt << 1] * mul_lazy[rt]) % m;
		mul_lazy[(rt << 1) | 1] = (mul_lazy[rt] * mul_lazy[(rt << 1) | 1]) % m;
		tree[(rt << 1) | 1] = (tree[(rt << 1) | 1] * mul_lazy[rt]) % m;
		sum_lazy[(rt << 1) | 1] = (sum_lazy[(rt << 1) | 1] * mul_lazy[rt]) % m;
		mul_lazy[rt] = 1;
	}
	if(sum_lazy[rt] != 0) 
	{
		sum_lazy[rt << 1] = (sum_lazy[rt] + sum_lazy[rt << 1]) % m;
		tree[rt << 1] = (tree[rt << 1] + sum_lazy[rt] * (mid - l + 1)) % m;
		sum_lazy[(rt << 1) | 1] = (sum_lazy[rt] + sum_lazy[(rt << 1) | 1]) % m;
		tree[(rt << 1) | 1] = (tree[(rt << 1) | 1] + sum_lazy[rt] * (r - mid)) % m;
		sum_lazy[rt] = 0;
	}
}
void add_sum(int rt,int l,int r,int ml,int mr,int m,long long k)
{
	if(r < ml || l > mr) return;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);
	if(ml <= l && mr >= r)
	{
		tree[rt] = (tree[rt] + k * (r - l + 1)) % m;
		sum_lazy[rt] = (sum_lazy[rt] + k) % m;
		return;
	}
	add_sum(rt << 1,l,mid,ml,mr,m,k);
	add_sum((rt << 1) | 1,mid + 1,r,ml,mr,m,k);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) | 1]) % m; 
}
void add_mul(int rt,int l,int r,int ml,int mr,int m,long long k)
{
	if(r < ml || l > mr) return;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);
	if(ml <= l && mr >= r)
	{
		tree[rt] = (tree[rt] * k) % m;
		mul_lazy[rt] = (mul_lazy[rt] * k) % m;
		sum_lazy[rt] = (sum_lazy[rt] * k) % m;
		return;
	}
	add_mul(rt << 1,l,mid,ml,mr,m,k);
	add_mul((rt << 1) | 1,mid + 1,r,ml,mr,m,k);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) | 1]) % m; 
}
long long myget(int rt,int l,int r,int ml,int mr,int m)
{
	if(r < ml || l > mr) return 0;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);
	if(ml <= l && mr >= r) return tree[rt];
	return (myget(rt << 1,l,mid,ml,mr,m) + myget((rt << 1) | 1,mid + 1,r,ml,mr,m)) % m;
}
int main()
{
	int n,q;
	long long m;
	cin >> n >> q >> m;
	init(n,m);
	for(int i = 1;i <= q;i++)
	{
		int op;
		cin >> op;
		if(op == 1)
		{
			int x,y;
			long long k;
			cin >> x >> y >> k;
			add_mul(1,1,n,x,y,m,k);
		}
		else if(op == 2)
		{
			int x,y;
			long long k;
			cin >> x >> y >> k;
			add_sum(1,1,n,x,y,m,k);
		}
		else
		{
			int x,y;
			cin >> x >> y;
			cout << myget(1,1,n,x,y,m) << '\n';
		}
	}
	return 0;
}
