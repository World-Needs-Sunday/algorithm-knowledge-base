#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		long long sum = 0;
		multiset<long long> ms;
		for (int i = 1; i <= n; i++)
		{
			int x;
			cin >> x;
			sum += x;
			ms.insert(x);
		}
		if (sum <= 0) cout << -1 << '\n';
		else
		{
			sum = 0;
			for (int i = 0; i < n; i++) {
				auto it = ms.lower_bound(1 - sum);
				sum += *it;
				ms.erase(it);
				cout << sum << ' ';
			}
			cout << '\n';
		}
	}
	return 0;
}
