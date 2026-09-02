#include<bits/stdc++.h>
using namespace std;
int n, m;
vector<long long> sum;
long long ans = -0x3f3f3f3f3f3f3f3f;
int main()
{
	/*P1714*/
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	cin >> n >> m;
	sum.resize(n + 1,0);
	for(int i = 1;i <= n;i++) 
	{
		long long x;
		cin >> x;
		sum[i] = sum[i - 1] + x;
	}
	deque<pair<long long ,int>> que; 
	for(int i = 1;i <= n;i++)
	{
		if(!que.empty())
		{
			if(i - que.front().second >= m) que.pop_front();
		}
		
		while(!que.empty() && que.back().first >= sum[i - 1]) que.pop_back();
		que.emplace_back(sum[i - 1],i);
		
		ans = max(sum[i] - que.front().first , ans);
	}
	cout << ans;
	return 0;
} 
