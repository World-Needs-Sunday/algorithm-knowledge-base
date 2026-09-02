#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,V,M;
	cin >> N >> V >> M;
	vector<vector<long long>> dp(V + 1,vector<long long>(M + 1,0));
	for(int k = 0;k < N;k++)
	{
		int v,m,w;
		cin >> v >> m >> w;
		for(int i = V;i >= v;i--)
		{
			for(int j = M;j >= m;j--)
			{
				dp[i][j] = max(max(dp[i - 1][j] , dp[i][j - 1]) , dp[i][j]);
				dp[i][j] = max(dp[i - v][j - m] + w,dp[i][j]); 
			}
		}
	}
	cout << dp[V][M];
} 
