#include<bits/stdc++.h>
using namespace std;
int main()
{
	int n, m, q;
	cin >> n >> m >> q;
	vector<vector<long long>> A(n + 2, vector<long long>(m + 2));
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cin >> A[i][j];
		}
	}
	for (int i = n; i >= 1; i--)
	{
		for (int j = m; j >= 1; j--)
		{
			A[i][j] = A[i][j] - A[i - 1][j] - A[i][j - 1] + A[i - 1][j - 1];
		}
	}
	while (q--)
	{
		int x1, y1, x2, y2;
		long long k;
		//(x1,y1)为左上角的点，(x2,y2)为右下角的点。
		cin >> x1 >> y1 >> x2 >> y2 >> k ;
		A[x1][y1] += k;
		A[x1][y2 + 1] -= k;
		A[x2 + 1][y1] -= k;
		A[x2 + 1][y2 + 1] += k;
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			A[i][j] = A[i][j] + A[i - 1][j] + A[i][j - 1] - A[i - 1][j - 1];
			cout << A[i][j] << ' ';
		}
		cout << '\n';
	}
	return 0;
}
