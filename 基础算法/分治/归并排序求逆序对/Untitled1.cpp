#include<bits/stdc++.h>
using namespace std;
int n;
vector<int> A;
vector<int> tmp;
long long fn(int l, int r)
{
	if (l == r) return 0;
	long long ans = 0;
	int mid = l + ((r - l) >> 1);
	ans += fn(l, mid);
	ans += fn(mid + 1, r);
	int idx = l, id1 = l, id2 = mid + 1;
	while (id1 != mid + 1 && id2 != r + 1)
	{
		if (A[id1] <= A[id2]) tmp[idx++] = A[id1++];
		else
		{
			tmp[idx++] = A[id2++];
			ans += mid - id1 + 1;
		}
	}
	while(id1 != mid + 1) tmp[idx++] = A[id1++];
	while (id2 != r + 1) tmp[idx++] = A[id2++];
	for (int i = l; i <= r; i++) A[i] = tmp[i];
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n;
	A.resize(n + 1, 0);
	tmp.resize(n + 1, 0);
	for (int i = 1; i <= n; i++) cin >> A[i];
	cout << fn(1, n);
	return 0;
}
