#include<bits/stdc++.h>
using namespace std;
vector<long long>A;
void dfs(int k, int n, long long sum, long long m,vector<long long>& tmp)
{
	if (k > n)
	{
		tmp.emplace_back(sum);
		return;
	}
	dfs(k + 1, n, sum, m,tmp);
	if (sum + A[k] <= m) dfs(k + 1, n, sum + A[k], m,tmp);
}
long long fn(int n, long long m)
{

	long long cnt = 0;
	int tmp_sz = n / 2;
	vector<long long> tmp1, tmp2;
	dfs(tmp_sz + 1, n, 0, m, tmp2);
	dfs(1, tmp_sz, 0, m, tmp1);

	sort(tmp1.begin(), tmp1.end());
	sort(tmp2.begin(), tmp2.end());

	int j = tmp2.size() - 1;
	for (int i = 0; i < tmp1.size(); i++)
	{
		while (j >= 0 && tmp1[i] + tmp2[j] > m) --j;
		cnt += j + 1;
	}
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
