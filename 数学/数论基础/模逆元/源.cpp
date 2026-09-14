#include<bits/stdc++.h>
using namespace std;
long long _gcd(long long a, long long b)
{
	return b ? _gcd(b, a % b) : a;
}
long long fastpow(long long a, long long b, long long q)
{
	if (b == 0) return 1;
	long long ans = 1;
	long long gap = a;
	while (b)
	{
		if (b & 1) ans = (ans * gap) % q;
		gap = (gap * gap) % q;
		b >>= 1;
	}
	return ans;
}
long long inv(long long a, long long q)
{
	if (_gcd(a, q) == 1) return fastpow(a, q - 2, q);
	return -1;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//q要为质数
	long long a, q;
	cin >> a >> q;
	cout << inv(a, q);
	return 0;
}
