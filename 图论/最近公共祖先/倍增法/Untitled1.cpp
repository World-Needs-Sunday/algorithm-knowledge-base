#include<bits/stdc++.h>
using namespace std;
int N, M;
vector<vector<int>> A;
vector<vector<int>> dp;
vector<int> h;
int len = 1;
void dfs(int rt, int t_len, vector<int>& tmp)
{
	if (t_len > len) len = t_len;
	h[rt] = t_len;
	for (int i : A[rt])
	{
		tmp[i] = rt;
		dfs(i, t_len + 1, tmp);
	}
}
void init(int rt)
{
	vector<int> tmp(N + 1);
	h.resize(N + 1);
	tmp[rt] = rt;
	dfs(rt, 1, tmp);
	int sz = 1;
	while (len >= ((long long)1 << sz)) ++sz;
	len = sz;

	dp.resize(N + 1, vector<int>(len + 1));
	for (int i = 1; i <= N; i++) dp[i][0] = tmp[i];
	for (int k = 1; k <= len; k++)
	{
		for (int i = 1; i <= N; i++)
		{
			dp[i][k] = dp[dp[i][k - 1]][k - 1];
		}
	}
}
int query(int a, int b)
{
	if (h[a] != h[b])
	{
		if (h[b] > h[a])
		{
			int t_h = h[b] - h[a];
			for (int i = 0; i <= len; i++)
			{
				if (t_h & 1) b = dp[b][i];
				t_h >>= 1;
			}
		}
		else
		{
			int t_h = h[a] - h[b];
			for (int i = 0; i <= len; i++)
			{
				if (t_h & 1) a = dp[a][i];
				t_h >>= 1;
			}
		}
	}
	if (a == b) return a;
	for (int i = len; i >= 0; i--)
	{
		if (dp[a][i] != dp[b][i])
		{
			a = dp[a][i];
			b = dp[b][i];
		}
	}
	return dp[a][0];
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> N >> M;
	vector<bool> vis(N + 1, false);
	A.resize(N + 1);
	for (int i = 1; i <= N; i++)
	{
		int n;
		cin >> n;
		A[i].resize(n);
		for (int j = 0; j < n; j++)
		{
			cin >> A[i][j];
			vis[A[i][j]] = true;
		}
	}
	int rt = 1;
	while (vis[rt]) rt++;
	init(rt);

	for (int i = 1; i <= M; i++)
	{
		int a, b;
		cin >> a >> b;
		cout << query(a, b) << '\n';
	}
	return 0;
}
