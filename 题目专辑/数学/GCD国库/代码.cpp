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
		int n, x;
		cin >> n >> x;
		vector<int> A(n + 1);
		for(int i = 1;i <= n;i++) cin >> A[i];
		
		long long ans = 0;
		for (int i = 2; i * i <= x; i++)
		{
			if (x % i == 0)
			{
				while (x % i == 0) x /= i;
				long long tmp = 0;
				for (int j = 1; j <= n; j++)
				{
					if (A[j] % i == 0)
					{
						tmp += A[j];
					}
				}
				ans = max(ans, tmp);
			}
		}
		if (x > 1)
		{
			long long tmp = 0;
			for (int j = 1; j <= n; j++)
			{
				if (A[j] % x == 0)
				{
					tmp += A[j];
				}
			}
			ans = max(ans, tmp);
		}
		cout << ans << '\n';
	}
	return 0;
}
