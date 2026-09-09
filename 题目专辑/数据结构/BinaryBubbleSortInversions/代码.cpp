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
		vector<char> a(n + 1), s(n + 1);
		for (int i = 1; i <= n; i++) cin >> a[i];
		for (int i = 1; i <= n; i++) cin >> s[i];

		long long inv = 0;
		int cnt0 = 0, cnt1 = 0;
		vector<int> count(n + 1);
		vector<long long> A(1);

		for (int i = 1; i <= n; i++)
		{
			if (a[i] == '0')
			{
				cnt0++;
				inv += cnt1;
				count[cnt1]++;
				A.emplace_back(cnt1);
			}
			else cnt1++;
		}

		for (int i = 1; i <= n; i++) count[i] += count[i - 1];

		int lazy0 = 0, lazy1 = 0;
		cout << inv << ' ';
		for (int i = 1; i <= n; i++)
		{
			if (inv)
			{
				if (s[i] == '1')
				{
					lazy1++;
					//可以交的起的数量
					int ct = cnt0 - count[lazy1 - 1] - lazy0;
					if (ct > 0) inv -= ct;
				}
				else
				{
					lazy0++;
					int idx = cnt0 - lazy0 + 1;
					if (A[idx] - lazy1 > 0) inv -= A[idx] - lazy1;
				}
			}
			cout << inv << ' ';
		}
		cout << '\n';
	}


	return 0;
}
