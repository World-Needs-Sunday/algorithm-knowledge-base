#include<bits/stdc++.h>
using namespace std;
vector<pair<long long, long long>> A,tmp1,tmp2;
long long d_2;
void init(int n)
{
	A.assign(n + 1, pair<long long, long long>{});
	tmp1.assign(n + 1, pair<long long, long long>{});
	tmp2.assign(n + 1, pair<long long, long long>{});
	d_2 = LLONG_MAX;
}

inline long long dist_2(const pair<long long, long long>& a, const pair<long long, long long>& b)
{
	return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

void find_min(int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			long long k = tmp2[j].second - tmp2[i].second;
			if (k * k > d_2) break;
			d_2 = min(d_2, dist_2(tmp2[i], tmp2[j]));
		}
	}
}

long long sovle(int l, int r)
{
	if (l == r) return A[l].first;
	if (l + 1 == r)
	{
		long long nxt = A[r].first;
		d_2 = min(d_2, dist_2(A[l], A[r]));
		if (A[l].second > A[r].second) swap(A[l], A[r]);
		return nxt;
	}
	int mid = l + (r - l) / 2;
	long long mid_x = sovle(l, mid);
	long long nxt = sovle(mid + 1, r);
	int id_l = l, id_r = mid + 1;
	int idx_tmp1 = l, idx_tmp2 = 1;
	while (id_l <= mid && id_r <= r)
	{
		if (A[id_l].second <= A[id_r].second)
		{
			tmp1[idx_tmp1++] = A[id_l];
			long long k = A[id_l].first - mid_x;
			if (k * k <= d_2) tmp2[idx_tmp2++] = A[id_l];
			id_l++;
		}
		else
		{
			tmp1[idx_tmp1++] = A[id_r];
			long long k = A[id_r].first - mid_x;
			if (k * k <= d_2) tmp2[idx_tmp2++] = A[id_r];
			id_r++;
		}
	}
	while (id_l <= mid)
	{
		tmp1[idx_tmp1++] = A[id_l];
		long long k = A[id_l].first - mid_x;
		if (k * k <= d_2) tmp2[idx_tmp2++] = A[id_l];
		id_l++;
	}
	while (id_r <= r)
	{
		tmp1[idx_tmp1++] = A[id_r];
		long long k = A[id_r].first - mid_x;
		if (k * k <= d_2) tmp2[idx_tmp2++] = A[id_r];
		id_r++;
	}
	for (int i = l; i <= r; i++) A[i] = tmp1[i];
	find_min(idx_tmp2);
	return nxt;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	init(n);
	for (int i = 1; i <= n; i++) cin >> A[i].first >> A[i].second;
	sort(A.begin() + 1, A.end());
	sovle(1, n);
	double ans = sqrt(d_2);
	cout << fixed << setprecision(4) << ans;
	return 0;
}
