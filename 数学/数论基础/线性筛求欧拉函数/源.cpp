#include<bits/stdc++.h>
using namespace std;
vector<int> prime, phi;
void phi_sieve(int n)
{
	phi.assign(n + 1, 0);
	prime.clear();
	phi[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		if (!phi[i])
		{
			phi[i] = i - 1;
			prime.emplace_back(i);
		}
		for (int j = 0; prime[j] <= n / i; j++)
		{
			if (i % prime[j]) phi[i * prime[j]] = (prime[j] - 1) * phi[i];
			else
			{
				phi[i * prime[j]] = prime[j] * phi[i];
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
