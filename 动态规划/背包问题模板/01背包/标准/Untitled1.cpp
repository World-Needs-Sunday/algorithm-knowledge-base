#include<bits/stdc++.h>
using namespace std;
int main()
{
	int n,m;
	cin >> n >> m;
	vector<int> W(n + 1);
	vector<int> C(n + 1);
	for(int i = 1;i <= n;i++) cin >> W[i] >> C[i];
	
	vector<vector<int>> dp(n + 1,vector<int>(m + 1,0));
	for(int i = 1;i <= n;i++)
	{
		for(int j = 0;j <= m;j++)
		{
			if(j >= W[i]) dp[i][j] =max(dp[i - 1][j - W[i]] + C[i] , dp[i - 1][j]); 
			else dp[i][j] = dp[i - 1][j]; 
		}
	}
	cout << dp[n][m];
	return 0;
}
