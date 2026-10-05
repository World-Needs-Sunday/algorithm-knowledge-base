#include<bits/stdc++.h>
using namespace std;
vector<char> ch;
vector<vector<int>> son;
vector<int> sk,fa;
vector<long long> dp;
int top;
void init(int n)
{
	ch.assign(n + 1, 0);
	son.assign(n + 1, vector<int>());
	sk.assign(n + 1, 0);
	top = 0;
	fa.assign(n + 1, 0);
	dp.assign(n + 1, 0);
}
inline void push(int x)
{
	sk[++top] = x;
}
inline void pop()
{
	--top;
}
void dfs(int i)
{
	if (ch[i] == '(')
	{
		push(i);
		dp[i] = 0;
		for (int j : son[i]) dfs(j);
		pop();
	}
	else
	{
		if (top != 0)
		{
			int t = sk[top];
			pop();
			dp[i] += 1 + dp[fa[t]];
			for (int j : son[i]) dfs(j);
			push(t);
		}
		else
		{
			for (int j : son[i]) dfs(j);
		}

		if (ch[fa[i]] == '(' && sk[top] != fa[i]) dp[i] += 1 + dp[fa[fa[i]]];
	}
}
void dfs2(int i)
{
	for (int j : son[i])
	{
		dp[j] += dp[i];
		dfs2(j);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	init(n);
	for (int i = 1; i <= n; i++) cin >> ch[i];
	for (int i = 2; i <= n; i++)
	{
		int x;
		cin >> x;
		son[x].emplace_back(i);
		fa[i] = x;
	}
	dfs(1);
	dfs2(1);
	long long ans = 0;
	for (int i = 1; i <= n; i++) ans ^= i * dp[i];
	cout << ans;
	return 0;
}
