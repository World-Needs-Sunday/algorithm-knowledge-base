#include<bits/stdc++.h>
using namespace std;
int n;
unsigned int m, k;
vector<int> A;
vector<unsigned int> tmp, cnt;
void init(vector<unsigned int>& val)
{
	for (int i = 0; i < n; i++) val[i] = (unsigned int)A[i] ^ ((unsigned int)1 << 31);
	unsigned int nmax = 0;
	tmp.assign(n, 0);
	cnt.assign(k, 0);
	for (int i = 0; i < n; i++) nmax = max(nmax, val[i]);
	m = 1;
	while (nmax >= k)
	{
		nmax /= k;
		++m;
	}
}

void base_sort()
{
	vector<unsigned int> val(n, 0);
	init(val);
	unsigned int base = 1;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < k; j++) cnt[j] = 0;
		for (int j = 0; j < n; j++) cnt[val[j] / base % k]++;
		for (int j = 1; j < k; j++) cnt[j] += cnt[j - 1];
		for (int j = n - 1; j >= 0; j--)
		{
			tmp[cnt[val[j] / base % k] - 1] = val[j];
			--cnt[val[j] / base % k];
		}
		for (int j = 0; j < n; j++) val[j] = tmp[j];
		base *= k;
	}

	for (int i = 0; i < n; i++) A[i] = val[i] ^ ((unsigned int)1 << 31);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n >> k;
	A.assign(n ,0);
	for (int i = 0; i < n; i++) cin >> A[i];
	base_sort();
	for (int i = 0; i < n; i++) cout << A[i] << ' ';
	return 0;
}