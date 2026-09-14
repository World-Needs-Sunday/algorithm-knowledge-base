#include<bits/stdc++.h>
using namespace std;
vector<long long> a, r;
void init(int n)
{
	a.assign(n + 1, 0);
	r.assign(n + 1, 0);
}
long long _gcd(long long a, long long b)
{
	return b ? _gcd(b, a % b) : a;
}
long long fastpow(long long a, long long b, long long q)
{
	long long ans = 1;
	long long gap = a % q;
	while (b)
	{
		if (b & 1) ans = (ans * gap) % q;
		gap = (gap * gap) % q;
		b >>= 1;
	}
	return ans;
}
long long CRT(int n,long long M)
{
	long long ans = 0;
	for (int i = 1; i <= n; i++)
	{
		long long x = a[i] % M;
		long long Mi = M / r[i];
		long long ti = fastpow(Mi, r[i] - 2, r[i]);
		x = (x * (Mi % M)) % M;
		x = (x * ti) % M;
		ans = (ans + x) % M;
	}
	return (ans % M + M) % M;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//所有的r都互质，并且r都为质数
	int n;
	long long M = 1;
	cin >> n;
	init(n);
	for (int i = 1; i <= n; i++)
	{
		cin >> r[i] >> a[i];
		M *= r[i];
	}
	cout << CRT(n, M);
	return 0;
}
