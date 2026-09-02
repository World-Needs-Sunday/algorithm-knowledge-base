#include<bits/stdc++.h>
using namespace std;
int N , W;
vector<long long> dp;
vector<pair<long long , long long>> A;
/*洛谷P1776*/
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> N >> W;
	dp.resize(W + 1, 0);
	for (int i = 1; i <= N; i++)
	{
		long long u, w, m;
		cin >> u >> w >> m;
		for (int k = 1; k <= m; k <<= 1)
		{
			A.emplace_back(u * k , w * k);
			m -= k;
		}
		if (m) A.emplace_back(u * m, w * m);
	}

	for (int i = 0; i < A.size(); i++)
	{
		for (int j = W; j >= 1; j--)
		{
			if (j >= A[i].second) dp[j] = max(dp[j], dp[j - A[i].second] + A[i].first);
		}
	}
	cout << dp[W];
	return 0;
}
