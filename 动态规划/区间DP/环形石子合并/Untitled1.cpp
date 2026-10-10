#include<bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;
vector<int> A;
vector<vector<int>> mindp;
vector<vector<int>> maxdp;
vector<int> nsum;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	A.resize(2 * n + 1,0);
	nsum.resize(2 * n + 1,0);
	mindp.resize(n + 1, vector<int>(2 * n + 1, INF));
	maxdp.resize(n + 1, vector<int>(2 * n + 1, 0));
	for (int i = 1; i <= n; i++)
	{
		cin >> A[i];
		A[i + n] = A[i];
	}
	for (int i = 1; i <= 2 * n; i++)
	{
		nsum[i] = nsum[i - 1] + A[i];
		mindp[1][i] = 0;
	}

	for (int len = 2; len <= n; len++)
	{
		for (int l = 1; l <= 2 * n - len + 1; l++)
		{
			for (int k = l + 1; k < l + len; k++)
			{
				mindp[len][l] = min(mindp[len][l], mindp[k - l][l] + mindp[len - k + l][k] + nsum[l + len - 1] - nsum[l - 1]);
				maxdp[len][l] = max(maxdp[len][l], maxdp[k - l][l] + maxdp[len - k + l][k] + nsum[l + len - 1] - nsum[l - 1]);
			}
		}
	}

	int minarr = INF;
	int maxarr = 0;
	for (int i = 1; i <= n; i++)
	{
		minarr = min(minarr, mindp[n][i]);
		maxarr = max(maxarr, maxdp[n][i]);
	}
	cout << minarr << endl << maxarr;
	
	return 0;
}
