#include<bits/stdc++.h>
using namespace std;
inline int read()
{
	int x = 0, f = 1; char ch = getchar();
	while (ch < '0' || ch>'9') { if (ch == '-') f = -1; ch = getchar(); }
	while (ch >= '0' && ch <= '9') { x = x * 10 + ch - 48; ch = getchar(); }
	return x * f;
}
inline void write(long long x)
{
	static char buf[30];
	int p = 0;
	if (x < 0) { putchar('-'); x = -x; }
	if (x == 0) buf[p++] = '0';
	while (x) { buf[p++] = x % 10 + 48; x /= 10; }
	while (p--) putchar(buf[p]);
}

vector<int> A;
int getnum(int l, int r)
{
	int a = A[l], b = A[r], c = A[l + (r - l) / 2];
	if (a >= b)
	{
		if (b >= c) return b;
		else
		{
			if (a >= c) return c;
			else return a;
		}
	}
	else
	{
		if (a >= c) return a;
		else
		{
			if (b >= c) return c;
			else return b;
		}
	}
}
int fast_k(int l, int r, int k)
{
	if (l == r) return A[l];
	int ml = l, mr = r;
	int q = getnum(l, r);
	while (ml <= mr)
	{
		while (A[ml] < q) ++ml;
		while (A[mr] > q) --mr;
		if (ml <= mr) swap(A[ml++], A[mr--]);
	}
	if (k <= mr) return fast_k(l, mr, k);
	if (k >= ml) return fast_k(ml, r, k);
	return q;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, k;
	n = read(), k = read();
	A.resize(n + 1);
	for (int i = 1; i <= n; i++) A[i] = read();
	write(fast_k(1, n, k));
	return 0;
}
