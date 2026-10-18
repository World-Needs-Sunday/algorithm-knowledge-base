#include<bits/stdc++.h>
using namespace std;
vector<int> a,vis,tim;
int ntime;
inline int read()
{
	long long x = 0, f = 1; char ch = getchar();
	while (ch < '0' || ch>'9') { if (ch == '-') f = -1; ch = getchar(); }
	while (ch >= '0' && ch <= '9') { x = x * 10 + ch - 48; ch = getchar(); }
	return x * f;
}
inline void write(unsigned long long x)
{
	static char buf[30];
	int p = 0;
	if (x == 0) buf[p++] = '0';
	while (x) { buf[p++] = x % 10 + 48; x /= 10; }
	while (p--) putchar(buf[p]);
}

void init(int n)
{
	a.assign(n + 1, 0);
	int big = 1;
	while (big < n + 1) big <<= 1;
	vis.assign(big, 0);
	tim.assign(big, 0);
	ntime = 0;
}

inline void update(int x)
{
	if (tim[x] != ntime)
	{
		tim[x] = ntime;
		vis[x] = 0;
	}
}

unsigned long long fn(int l, int r, int k)
{
	++ntime;
	unsigned long long p = 0;
	int cnt = 0;
	int mr = l - 1;

	for (int ml = l; ml <= r; ++ml)
	{
		while (mr < r && cnt < k)
		{
			++mr;
			if (a[mr] < k)
			{
				update(a[mr]);
				if (vis[a[mr]] == 0) ++cnt;
				++vis[a[mr]];
			}
		}

		if (cnt == k) p += (unsigned long long)r - mr + 1;

		if (mr >= ml && a[ml] < k)
		{
			--vis[a[ml]];
			if (vis[a[ml]] == 0) --cnt;
		}
	}
	return p;
}
int main()
{
	int n;
	unsigned long long ans = 0;
	n = read();
	init(n);
	for (int i = 1; i <= n; i++) a[i] = read();
	for (int b = 0; b <= 30; b++)
	{
		unsigned long long cnt = (unsigned long long)n * (n + 1) / 2;
		int k = 1 << b;
		for (int l = 1; l <= n; l++)
		{
			if (!(a[l] & k))
			{
				int r = l;
				while (r < n && !(a[r + 1] & k)) ++r;

				unsigned long long len = r - l + 1;
				cnt -= len * (len + 1) / 2;

				if (k <= n)
				{
					unsigned long long t = fn(l, r, k);
					ans += t * k;
				}

				l = r;
			}
		}
		ans += cnt * k;
	}
	write(ans);
	return 0;
}