#include<bits/stdc++.h>
using namespace std;
const int nmax = 1e6 + 10;
int sk_idx[nmax], id;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		id = 0;
		int n;
		cin >> n;
		vector<int> A(n + 1);
		for (int i = 1; i <= n; i++) cin >> A[i];
		A.emplace_back(-1e9);
		long long ans = 0;
		for (int i = 1; i <= n + 1; i++)
		{
			while (id && A[i] < A[sk_idx[id - 1]])
			{
				int top = A[sk_idx[id - 1]];
				--id;
				if (id) ans = max((long long)(i - sk_idx[id - 1] - 1) * top, ans);
				else ans = max((long long)(i - 1) * top, ans);
			}
			sk_idx[id++] = i;
		}
		cout << ans << '\n';
	}
	return 0;
}
