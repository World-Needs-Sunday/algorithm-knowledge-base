#include<bits/stdc++.h>
using namespace std;
vector<pair<int,int>> A[101];
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int m,n,sz = 0;
	cin >> m >> n;
	vector<long long> dp(m + 1,0);
	for(int i = 0;i < n;i++)
	{
		int a,b,c;
		cin >> a >> b >> c;
		A[c].emplace_back(a,b);
		sz = max(sz,c);
	}
	for(int i = 1;i <= sz;i++)
	{
		for(int j = m;j > 0;j--)
		{
			for(int z = 0;z < A[i].size();z++)
			{
				if(j >= A[i][z].first) dp[j] = max(dp[j - A[i][z].first] + A[i][z].second, dp[j]);
			}
		}
	}
	cout << dp[m]; 
	return 0;	
} 
