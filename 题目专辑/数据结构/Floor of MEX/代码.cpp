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
		vector<int> A(n, 0);
		vector<int> ans;
		ans.reserve(n);
		for (int i = 1; i <= n; i++)
		{
			int x;
			cin >> x;
			if (1LL * i * x < n) A[i * x] += 1;
			if (1LL * (x + 1) * i < n) A[(x + 1) * i] -= 1;
		}
		if (!A[0]) ans.emplace_back(0);
		for (int i = 1; i < n; i++)
		{
			A[i] += A[i - 1];
			if (!A[i]) ans.emplace_back(i);
		}
		cout << ans.size() << '\n';
		for (int i : ans) cout << i << ' ';
		cout << '\n';
	}
}
