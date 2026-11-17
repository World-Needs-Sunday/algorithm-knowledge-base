#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
const int nmax = 3e5 + 5;
vector<int> idx[nmax],col;

//读取数量
int get(int l, int r, int c)
{
	return upper_bound(idx[c].begin(), idx[c].end(), r) - lower_bound(idx[c].begin(), idx[c].end(), l);
}

//交换
void _swap(int a)
{
	int b = a + 1;
	//相邻交换不会破坏有序性
	if (col[a] == col[b]) return;
	int ca = col[a], cb = col[b];
	int idx_a = lower_bound(idx[ca].begin(), idx[ca].end(), a) - idx[ca].begin();
	idx[ca][idx_a] = b;
	int idx_b = lower_bound(idx[cb].begin(), idx[cb].end(), b) - idx[cb].begin();
	idx[cb][idx_b] = a;
	swap(col[a], col[b]);
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n,m;
	cin >> n >> m;
	col.assign(n + 1, 0);
	for (int i = 1; i <= n; i++)
	{
		int x;
		cin >> x;
		idx[x].emplace_back(i);
		col[i] = x;
	}
	for (int i = 1; i <= m; i++)
	{
		int op;
		cin >> op;
		if (op == 1)
		{
			int l, r, c;
			cin >> l >> r >> c;
			cout << get(l, r, c) << '\n';
		}
		else
		{
			int x;
			cin >> x;
			_swap(x);
		}
	}
	return 0;
}