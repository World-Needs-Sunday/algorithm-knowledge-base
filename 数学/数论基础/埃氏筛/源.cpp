#include<bits/stdc++.h>
using namespace std;
vector<bool> sieve_vis(int n)
{
	vector<bool> prime_vis(n + 1,true);
	for (int i = 2; i * i <= n; i++)
	{
		if (!prime_vis[i]) continue;
		for (int j = i * i; j <= n; j += i)
		{
			prime_vis[j] = false;
		}
	}
	return prime_vis;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	vector<bool> vis = move(sieve_vis(5000000));
	while (t--)
	{
		int x;
		cin >> x;
		if (vis[x]) cout << "是质数" << '\n';
		else cout << "是合数" << '\n';
	}
	return 0;
}
