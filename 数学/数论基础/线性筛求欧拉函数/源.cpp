#include<bits/stdc++.h>
using namespace std;
vector<int> prime, phi;
void phi_sieve(int n)
{
	vector<bool> prime_vis(n + 1, true);
	prime.clear();
	prime.reserve(n + 1);
	phi.assign(n + 1,0);
	phi[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		if (prime_vis[i])
		{
			prime.emplace_back(i);
			phi[i] = i - 1;
		}
		for (int j = 0; prime[j] <= n / i; j++)
		{
			prime_vis[i * prime[j]] = false;
			if (i % prime[j] == 0)
			{
				phi[i * prime[j]] = prime[j] * phi[i];
				break;
			}
			else phi[i * prime[j]] = (prime[j] - 1) * phi[i];
		}
	}
	prime.shrink_to_fit();
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, q;
	cin >> n >> q;
	phi_sieve(n);
	while (q--)
	{
		int n;
		cin >> n;
		if (phi[n] == n - 1) cout << n << "为质数\n";
		else cout << n << "为合数\n";
		cout << n << "的欧拉函数的值为: " << phi[n];
	}
	return 0;
}
