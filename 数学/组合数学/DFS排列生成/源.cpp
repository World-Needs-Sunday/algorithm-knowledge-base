#include<bits/stdc++.h>
using namespace std;
vector<int> tmp;
vector<bool> vis;
void init(int n)
{
	tmp.assign(n + 1, 0);
	vis.assign(n + 1, false);
}
void dfs(int k,int n)
{
	if (k > n)
	{
		//如果要打印数组中的值,则将tmp[i]当下标,如果是数组要打印字典序,则给先数组排序
		for (int i = 1; i <= n; i++) cout << tmp[i] << ' ';
		cout << '\n';
		return;
	}
	for (int i = 1; i <= n; i++)
	{
		if (vis[i]) continue;
		vis[i] = true;
		tmp[k] = i;
		dfs(k + 1, n);
		vis[i] = false;
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	init(n);
	dfs(1, n);
	return 0;
}
