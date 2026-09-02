#include<bits/stdc++.h>
using namespace std;
const int nmax = 1e6 + 10;
int sk_num[nmax],sk_idx[nmax], id;
void init()
{
	id = 0;
	sk_num[id] = -1e9;
	sk_idx[id] = 0;
	++id;
}
long long add(int x,int i)
{
	long long ans = 0;
	int new_idx = i;
	while(x < sk_num[id - 1])
	{
		ans = max(ans, (long long)sk_num[id - 1] * (i - sk_idx[id - 1]));
		new_idx = sk_idx[id - 1];
		--id;
	}
	sk_num[id] = x;
	sk_idx[id] = new_idx;
	++id;
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n;
		long long ans = 0;
		cin >> n;
		init();
		for (int i = 1; i <= n; i++)
		{
			int x;
			cin >> x;
			ans = max(add(x, i), ans);
		}
		ans = max(add(-1e9, n + 1), ans);
		cout << ans << '\n';
	}
	return 0;
}
