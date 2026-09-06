#include<bits/stdc++.h>
using namespace std;
long long _gcd(long long a, long long b)
{
	return b ? _gcd(b, a % b) : a;
}
long long _lcm(long long a, long long b)
{
	return a / _gcd(a, b) * b;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		long long a, b;
		cin >> a >> b;
		cout << _gcd(a, b) << ' ' << _lcm(a, b) << '\n';
	}
	return 0;
}
