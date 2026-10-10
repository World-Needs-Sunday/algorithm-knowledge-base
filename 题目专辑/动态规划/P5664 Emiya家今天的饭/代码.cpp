#include<bits/stdc++.h>
using namespace std;
const long long q = 998244353;
long long ans = 1;
vector<vector<long long>> A , dp;
vector<long long> sum_n;
void init(int n,int m)
{
	ans = 1;
	A.assign(n + 1, vector<long long>(m + 1, 0));
	sum_n.assign(n + 1, 0);
	dp.assign(n + 1, vector<long long>(2 * n + 2,0));
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	init(n, m);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cin >> A[i][j];
			A[i][j] %= q;
			sum_n[i] = (A[i][j] + sum_n[i]) % q;
		}
	}
	for (int i = 1; i <= n; i++) ans = (ans * (sum_n[i] + 1)) % q;
	dp[0][n] = 1;
	for (int j = 1; j <= m; j++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int k = 0; k <= 2 * n; k++)
			{
				dp[i][k] = dp[i - 1][k];
				if (k != 0) dp[i][k] = (dp[i][k] + (dp[i - 1][k - 1] * A[i][j]) % q) % q;
				if (k != 2 * n) dp[i][k] = (dp[i][k] + (dp[i - 1][k + 1] * ((sum_n[i] - A[i][j] + q) % q)) % q) % q;
			}
		}

		for (int i = n + 1; i <= 2 * n; i++) ans -= dp[n][i];
	}
	
	
	ans -= 1;
	while (ans < 0) ans += q;
	cout << ans;
	return 0;
}