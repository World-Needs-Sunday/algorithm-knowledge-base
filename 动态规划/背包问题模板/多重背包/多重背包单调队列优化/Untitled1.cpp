#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	long long n,W;
	cin >> n >> W;
	vector<long long> dp(W + 1,0);
	deque<pair<long long,long long>> que;
	while(n--)
	{
		long long v,w,m;
		cin >> v >> w >> m;
		for(long long i = 0;i < w;i++)
		{
			que.clear(); 
			for(long long j = i;j <= W;j += w)
			{
				if(!que.empty() && (j - que.front().first) / w > m) que.pop_front(); 
				while(!que.empty() && dp[j] >= que.back().second + (j - que.back().first) / w * v) que.pop_back();
				que.emplace_back(j,dp[j]);
				dp[j] = que.front().second + (j - que.front().first) / w * v;
			}
		}
	}
	cout << dp[W];
	return 0;
} 
