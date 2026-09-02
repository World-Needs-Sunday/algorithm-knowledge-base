#include<bits/stdc++.h>
using namespace std;
int main()
{
	int N,V;
	cin >> N >> V;
	vector<pair<int,int>> A(N + 2);
	for(int i = 1;i <= N;i++) cin >> A[i].first >> A[i].second;
	vector<vector<int>> dp(N + 2,vector<int>(V + 2,0));
	for(int i = N;i >= 1;i--)
	{
		for(int j = V;j >= 1;j--)
		{
			dp[i][j] = dp[i+1][j];
			if (j >= A[i].first) dp[i][j] = max(dp[i + 1][j - A[i].first] + A[i].second , dp[i + 1][j]);
		}
	}
	int j = V;
	for(int i = 1;i <= N;i++)
	{
		if(j >= A[i].first && dp[i][j] == dp[i + 1][j - A[i].first] + A[i].second)
		{
			cout << i << ' ';
			j -= A[i].first;
		}
	}
	return 0;
} 
