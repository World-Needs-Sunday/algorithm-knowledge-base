#include<bits/stdc++.h>
using namespace std;
vector<long long>A, tmp;
long long cnt;
void dfs1(int k,int n,long long sum,long long m)
{
	if (k > n)
	{
		tmp.emplace_back(sum);
		return;
	}
	dfs1(k + 1, n, sum, m);
	if (sum + A[k] <= m) dfs1(k + 1, n, sum + A[k], m);
}

int find(long long i)
{
	return upper_bound(tmp.begin(), tmp.end(), i) - tmp.begin();
}

void dfs2(int k, int n, long long sum, long long m)
{
	if (k > n)
	{
		cnt += find(m - sum);
		return;
	}
	dfs2(k + 1, n, sum, m);
	if (sum + A[k] <= m) dfs2(k + 1, n, sum + A[k], m);
}

long long fn(int n,long long m)
{
	tmp.clear();
	cnt = 0;
	int tmp_sz = n / 2;
	tmp.reserve((long long)1 << tmp_sz);
	dfs1(1, tmp_sz, 0, m);
	sort(tmp.begin(), tmp.end());
	dfs2(tmp_sz + 1, n, 0, m);
	return cnt;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	long long m;
	cin >> n >> m;
	A.assign(n + 1, 0);
	for (int i = 1; i <= n; i++) cin >> A[i];
	cout << fn(n, m);
	return 0;
}
