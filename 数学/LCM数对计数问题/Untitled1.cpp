#include<bits/stdc++.h>
using namespace std;
vector<pair<long long,long long>> AM;
long long ans;
void init(long long n)
{
	ans = 0;
	AM.clear();
	for(long long i = 2;i * i <= n;i++)
	{
		long long cnt = 0;
		while(n % i == 0)
		{
			++cnt;
			n /= i;
		}
		if(cnt) AM.emplace_back(i,cnt);
	}
	if(n > 1) AM.emplace_back(n,1);
}
void dfs(long long idx,long long a,long long b)
{
	if(idx == AM.size())
	{
		cout << a << " " << b << '\n';
		++ans;
		return;
	}
	vector<long long> tmp(AM[idx].second + 1);
	tmp[0] = 1;
	for(long long i = 1;i <= AM[idx].second;i++) tmp[i] = tmp[i - 1] * AM[idx].first;
	for(long long i = 0;i <= AM[idx].second;i++) dfs(idx + 1 , a * tmp[AM[idx].second] , b * tmp[i]);
	for(long long i = 0;i < AM[idx].second;i++) dfs(idx + 1 , a * tmp[i] , b * tmp[AM[idx].second]);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int t;
	cin >> t;
	for(int i = 1;i <= t;i++)
	{
		long long n;
		cin >> n;
		init(n);
		dfs(0,1,1);
		cout << "Case " << i << ": " << ans << '\n';
	}
	return 0;	
} 
