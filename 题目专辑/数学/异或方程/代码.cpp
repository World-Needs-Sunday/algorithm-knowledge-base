#include<bits/stdc++.h>
using namespace std;
__int128 check(const vector<__int128>& dp, int m, int n, __int128 sum)
{
	if (sum == 0)
	{
		for (int i = 0; i < 128; i++)
		{
			if (dp[i] == 0)
			{
				__int128 ans = i;
				if (ans < 1) ans += 128;
				if (ans >= 1 && ans <= m) return ans;
			}
		}
		return -1;
	}
	else
	{
		for (int i = 0; i < 128; i++)
		{
			if ((-dp[i]) % (128 * sum) == 0)
			{
				__int128 ans = -dp[i] / sum + i;
				if (ans >= 1 && ans <= m)
				{
					return ans;
				}
			}
		}
		return -1;
	}
}

inline void write(__int128 x)
{
	static char buf[42];
	int p = 0;
	if (x < 0) { putchar('-'); x = -x; }
	if (x == 0) buf[p++] = '0';
	while (x) { buf[p++] = x % 10 + 48; x /= 10; }
	while (p--) putchar(buf[p]);
	putchar('\n');
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n, m;
		__int128 sum = 0;
		cin >> n >> m;
		vector<long long> A(n + 1);
		vector<__int128> dp(128, 0);
		for (int i = 0; i <= n; i++)
		{
			cin >> A[i];
			if (i != 0) sum += A[i];
		}
		if (n == 0)
		{
			if (A[0] == 0) write(1);
			else write(-1);
		}
		else
		{
			for (int i = 0; i < 128; i++)
			{
				dp[i] += A[0];
				dp[i] += (__int128)A[1] * i;
				for (int j = 2; j <= n; j++)
				{
					dp[i] += (__int128)A[j] * (i ^ j);
				}
			}

			write(check(dp, m, n, sum));
		}
	}
	return 0;
}
