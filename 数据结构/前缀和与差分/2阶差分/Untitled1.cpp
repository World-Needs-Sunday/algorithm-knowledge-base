#include<bits/stdc++.h>
using namespace std;
inline int lowbit(int x)
{
	return x & -x;
}

struct treearr
{
	vector<long long> arr;
	int n;
	treearr() {}
	treearr(int k)
	{
		n = k;
		arr.assign(n + 1, 0);
	}

	void add(int idx, long long k)
	{
		while (idx <= n)
		{
			arr[idx] += k;
			idx += lowbit(idx);
		}
	}

	long long get(int idx)
	{
		long long ans = 0;
		while (idx)
		{
			ans += arr[idx];
			idx -= lowbit(idx);
		}
		return ans;
	}
};
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int N, M;
	cin >> N >> M;
	treearr S1(N), S2(N);
	vector<long long> a(N + 1, 0);
	for (int i = 1; i <= N; i++) cin >> a[i];
	for (int k = 1; k <= 2; k++)
	{
		for (int i = N; i >= 1; i--)
		{
			a[i] -= a[i - 1];
		}
	}
	for (int i = 1; i <= N; i++)
	{
		S1.add(i, a[i]);
		S2.add(i, a[i] * i);
	}
	

	for (int i = 1; i <= M; i++)
	{
		int opt;
		cin >> opt;
		if (opt == 1)
		{
			int l, r;
			long long K, D;
			cin >> l >> r >> K >> D;
			S1.add(l, K);
			S2.add(l, K * l);
			S1.add(l + 1, D - K);
			S2.add(l + 1, (D - K) * (l + 1));

			S1.add(r + 1, D * -(r - l + 1) - K);
			S2.add(r + 1, (D * -(r - l + 1) - K) * (r + 1));
			S1.add(r + 2, K + D * (r - l));
			S2.add(r + 2, (K + D * (r - l)) * (r + 2));
		}
		else if (opt == 2)
		{
			int p;
			cin >> p;
			cout << (p + 1) * S1.get(p) - S2.get(p) << '\n';
		}
	}
	return 0;
}
