#include<bits/stdc++.h>
using namespace std;
bool check(int k,int n,const vector<int>& A)
{
	int gap = k * 2;
	int nmax = INT_MIN;
	for (int i = 0; i < n;)
	{
		int j = i + gap;
		int tmin = INT_MAX, tmax = INT_MIN;
		for (int l = i; l < min(j , n); l++)
		{
			tmin = min(tmin, A[l]);
			tmax = max(tmax, A[l]);
		}
		if (tmin < nmax) return false;
		nmax = tmax;
		i = min(j, n);
	}
	return true;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n, q;
		cin >> n >> q;
		vector<int> A(n);
		for (int i = 0; i < n; i++) cin >> A[i];
		
		if (is_sorted(A.begin(), A.end())) cout << 0 << '\n';
		else
		{
			int k = 1;
			while (!check(k, n, A)) k <<= 1;
			cout << k << '\n';
		}
	}
	return 0;
}
