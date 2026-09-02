#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	vector<int> A(n + 1);
	for (int i = 1; i <= n; i++) cin >> A[i];
	for (int i = n; i >= 1; i--)A[i] -= A[i - 1];
	long long pos = 0, neg = 0;
	for (int i = 2; i <= n; i++)
	{
		if (A[i] > 0) pos += A[i];
		else neg -= A[i];
	}
	cout << max(pos, neg) << '\n';
	cout << abs(pos - neg) + 1;
	return 0;
}
