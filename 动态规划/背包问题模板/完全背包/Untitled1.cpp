#include<bits/stdc++.h>
using namespace std;
int main()
{
	int n,m;
	cin >> n >> m;
	vector<pair<int,int>> A(n + 1);
	for(int i = 1;i <= n;i++) cin >> A[i].first >> A[i].second;
	
	vector<vector<int>> dp(n + 1,vector<int> (m + 1,0));
	for(int i = 1;i <= n;i++)
	{
		for(int j = 1;j <= m;j++)
		{
			if(j >= A[i].first) dp[i][j] = max(dp[i - 1][j],dp[i][j - A[i].first] + A[i].second);
			else dp[i][j] = dp[i - 1][j];
		}
	}
	cout << dp[n][m];
	return 0;
} 
