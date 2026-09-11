#include<bits/stdc++.h>
using namespace std;
vector<int> tmp;
void init(int m)
{
	tmp.assign(m + 1, 0);
}
void dfs(int k, int idx ,int m, int n)
{
	if (idx == m + 1)
	{
		//如果要打印数组中的值,则将tmp[i]当下标,如果是数组要打印字典序,则给先数组排序
		for (int i = 1; i <= m; i++) cout << tmp[i] << ' ';
		cout << '\n';
		return;
	}
	//选择,没有满
	if (idx <= m)
	{
		tmp[idx] = k;
		dfs(k + 1, idx + 1, m, n);
	}
	//不选择，剩余可填写 > 剩余需要填写
	if (n - k + 1 > m - idx + 1) dfs(k + 1, idx, m, n);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n,m;
	cin >> n >> m;
	init(m);
	dfs(1, 1, m, n);
	return 0;
}
