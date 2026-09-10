#include<bits/stdc++.h>
using namespace std;
const int sz = 1e5 * 31 + 10;
struct node
{
	int son[2];
	node() { son[0] = 0, son[1] = 0; }
};
node tree[sz];
int idx = 1;
void insert(int num)
{
	int cur = 1;
	for (int i = 31; i >= 0; --i)
	{
		int k = ((num >> i) & 1) ? 1 : 0;
		if (tree[cur].son[k] == 0) tree[cur].son[k] = ++idx;
		cur = tree[cur].son[k];
	}
}
int find(int num)
{
	int cur = 1;
	int ans = 0;
	for (int i = 31; i >= 0; --i)
	{
		int k = ((num >> i) & 1) ? 0 : 1;
		if (tree[cur].son[k] == 0) k = k ? 0 : 1;
		else ans += (1 << i);
		cur = tree[cur].son[k];
	}
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, q;
	cin >> n >> q;
	for (int i = 1; i <= n; i++)
	{
		int x;
		cin >> x;
		insert(x);
	}
	for (int i = 1; i <= q; i++)
	{
		int x;
		cin >> x;
		cout << find(x) << '\n';
	}
	return 0;
}
