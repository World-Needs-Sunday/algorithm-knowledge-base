#include<bits/stdc++.h>
using namespace std;
vector<int> v, w;
vector<vector<long long>> dp;
vector<long long> tmp;
void init(int n, int m, int k)
{
	v.assign(m + 1, 0);
	w.assign(m + 1, 0);
	dp.assign(n + 1, vector<long long>(k , LLONG_MIN));
	tmp.assign(k , 0);
	dp[0][0] = 0;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m, k;
	cin >> k >> n >> m;
	init(n, m, k);

	for (int i = 1; i <= m; i++) cin >> v[i] >> w[i];

	for (int i = 1; i <= m; i++)
	{
		for (int j = n; j >= v[i]; j--)
		{
			int l = 0, r = 0, idx = 0;
			while (idx < k && dp[j - v[i]][l] != LLONG_MIN && dp[j][r] != LLONG_MIN)
			{
				if (dp[j - v[i]][l] + w[i] >= dp[j][r])
				{
					tmp[idx] = dp[j - v[i]][l] + w[i];
					++idx, ++l;
				}
				else
				{
					tmp[idx] = dp[j][r];
					++idx, ++r;
				}
			}

			while (idx < k && dp[j - v[i]][l] != LLONG_MIN)
			{
				tmp[idx] = dp[j - v[i]][l] + w[i];
				++idx, ++l;
			}

			while (idx < k && dp[j][r] != LLONG_MIN)
			{
				tmp[idx] = dp[j][r];
				++idx, ++r;
			}

			for (int z = 0; z < idx; z++) dp[j][z] = tmp[z];
		}
	}

	long long sum = 0;
	for (int i = 0; i < k; i++)
	{
		if (dp[n][i] != LLONG_MIN) sum += dp[n][i];
	}
	cout << sum;

	return 0;

}