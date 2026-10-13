#include<bits/stdc++.h>
using namespace std;
vector<long double> k;
long double fn(long double x,int n)
{
	long double t = 1,ans  = 0;
	for (int i = 0; i <= n; i++)
	{
		ans += t * k[i];
		t *= x;
	}
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	long double L, R;
	cin >> n >> L >> R;
	k.assign(n + 1, 0);
	for (int i = n; i >= 0; i--) cin >> k[i];
	while (R - L > 1e-8)
	{
		long double l = L + (R - L) / 3;
		long double r = L + (R - L) / 3 * 2;
		if (fn(l, n) < fn(r, n)) L = l;
		else R = r;
	}
	cout << (L + R) / 2;
	return 0;
}