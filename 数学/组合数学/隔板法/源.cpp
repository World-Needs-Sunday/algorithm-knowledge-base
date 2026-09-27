#include<bits/stdc++.h>
using namespace std;

// 方法一：隔板法公式 C(n + k - 1, k - 1)
long long comb(int n, int k)
{
	long long res = 1;
	for (int i = 1; i <= k; i++)
		res = res * (n - k + i) / i;
	return res;
}

// 方法二：动态规划（递推求和）
// F(n, k) = sum_{i=0}^{n} F(i, k-1)
long long fn(int n, int k)
{
	vector<vector<long long>> dp(k + 1, vector<long long>(n + 1));
	dp[0][0] = 1;
	for (int i = 1; i <= k; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			for (int z = 0; z <= j; z++)
			{
				dp[i][j] += dp[i - 1][z];
			}
		}
	}
	return dp[k][n];
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, k;
	cin >> n >> k;
	cout << "方法一：F(n,k) = C(n + k - 1,k - 1)\n" << comb(n + k - 1, k - 1) << '\n';
	cout << "方法二：F(n,k) = sum_{i=0}^{n} F(i, k-1)\n" << fn(n, k) << '\n';
	return 0;
}
