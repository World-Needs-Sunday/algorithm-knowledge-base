#include<bits/stdc++.h>
using namespace std;
vector<int> prime;
vector<long long> g, f;
void f_sieve(int n)
{
	prime.clear();
	prime.reserve(n / 15 + 16);
	g.assign(n + 1, 0);
	f.assign(n + 1, 0);
	f[1] = g[1] = 1;
	vector<bool> vis(n + 1, false);
	for (int i = 2; i <= n; i++)
	{
		if (!vis[i])
		{
			g[i] = f[i] = i + 1;
			prime.emplace_back(i);
		}
		for (int j = 0; prime[j] <= n / i; j++)
		{
			long long m = prime[j] * i;
			vis[m] = true;
			if (i % prime[j])
			{
				g[m] = 1 + prime[j];
				f[m] = f[i] * g[m];
			}
			else
			{
				g[m] = g[i] * prime[j] + 1;
				f[m] = f[i] / g[i] * g[m];
				break;
			}
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, q;
	cin >> n >> q;
	f_sieve(n);
	for (int i = 1; i <= q; i++)
	{
		int x;
		cin >> x;
		cout << f[x] << '\n';
	}
	return 0;
}
