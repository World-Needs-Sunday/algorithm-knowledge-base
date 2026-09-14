#include<bits/stdc++.h>
using namespace std;
vector<int> prime;
vector<int> a, d;
void d_sieve(int n)
{
	prime.clear();
	prime.reserve(n / 15 + 16);
	a.assign(n + 1, 0);
	d.assign(n + 1, 0);
	d[1] = 1;
	vector<bool> vis(n + 1, false);
	for (int i = 2; i <= n; i++)
	{
		if (!vis[i])
		{
			prime.emplace_back(i);
			d[i] = 2;
			a[i] = 1;
		}
		for (int j = 0; prime[j] <= n / i; j++)
		{
			int m = prime[j] * i;
			vis[m] = true;
			if (i % prime[j])
			{
				a[m] = 1;
				d[m] = d[i] * 2;
			}
			else
			{
				a[m] = a[i] + 1;
				d[m] = d[i] / a[m] * (a[m] + 1);
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
	d_sieve(n);
	for (int i = 1; i <= q; i++)
	{
		int x;
		cin >> x;
		cout << d[x] << '\n';
	}
	return 0;
}
