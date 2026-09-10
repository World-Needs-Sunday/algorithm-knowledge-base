#include<bits/stdc++.h>
using namespace std;
const long long sz = (long long)1 << 18;
int g[sz + 10];
vector<long long> a;
long long check(int L,int n)
{
	long long k = (long long)1 << L;
	long long c2 = L;
	long long sum13 = 0;
	for (int i = 1; i <= n; i++)
	{		
		long long c1 = (a[i] % k) ? (k - (a[i] % k)) : 0;
		long long c3 = g[(a[i] + c1) >> L];
		long long sum = c1 + c3;
		int cnt = 1;
		while (true)
		{
			long long tc1 = c1 + ((long long)1 << L) * cnt;
			long long tc3 = g[((a[i] + c1) >> L) + cnt];
			sum = min(tc1 + tc3, sum);
			if (!((a[i] + tc1) & (a[i] + tc1 - 1)) || (((long long)1 << L) * cnt >= 34)) break;
			cnt++;
		}
		sum13 += sum;
	}
	return c2 + sum13;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	g[0] = 0;
	for (int i = 1; i <= sz; i++)
	{
		if (i & 1) g[i] = g[i - 1] + 1;
		else g[i] = g[i >> 1] + 1;
	}
	while (t--)
	{
		int n;
		cin >> n;
		a.assign(n + 1, 0);
		for (int i = 1; i <= n; i++) cin >> a[i];
		long long ans = LLONG_MAX;
		for (int i = 0; i <= 17; i++) ans = min(ans, check(i, n));
		cout << ans << '\n';
	}
	return 0;
}
