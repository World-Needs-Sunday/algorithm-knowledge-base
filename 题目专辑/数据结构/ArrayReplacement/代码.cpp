#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<long long> A(n + 1);
		for (int i = 1; i <= n; i++) cin >> A[i];
		for (int i = n; i >= 1; i--) A[i] -= A[i - 1];
		for (int i = 2; i <= n;)
		{
			int j = i;
			while (j + 1 <= n && ((A[i] & 1) == (A[j + 1] & 1))) j++;
			sort(A.begin() + i, A.begin() + j + 1);
			i = j + 1;
		}
		for (int i = 1; i <= n; i++)
		{
			A[i] += A[i - 1];
			cout << A[i] << ' ';
		}
		cout << '\n';
	}
	return 0;
}
