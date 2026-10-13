#include<bits/stdc++.h>
using namespace std;
const double TL = 0.9;
vector<int> cur_arr, best_arr;
long double curE = DBL_MAX, bestE = DBL_MAX;
mt19937 gen;
void init(int n)
{
	cur_arr.assign(n + 1, 0);
	best_arr.assign(n + 1, 0);
	curE = DBL_MAX, bestE = DBL_MAX;
	gen.seed(time(nullptr));
}
long double cala(int n, int m, vector<int>& arr)
{
	for (int i = 1; i <= n; i++) arr[i] += arr[i - 1];
	vector<vector<long double>> dp(m + 1, vector<long double>(n + 1,DBL_MAX));
	long double avg = (long double)arr[n] / m;
	for (int i = 1; i <= n; i++)
	{
		long double t = avg - (long double)arr[i];
		dp[1][i] = t * t;
	}
	for (int k = 2; k <= m; k++)
	{
		for (int i = k; i <= n; i++)//处理到的元素
		{
			for (int j = k; j <= i; j++)//新的一层的起点
			{
				long double t = avg - (long double)(arr[i] - arr[j - 1]);
				dp[k][i] = min(dp[k][i], dp[k - 1][j - 1] + t * t);
			}
		}
	}
	for (int i = n; i >= 1; i--) arr[i] -= arr[i - 1];
	return dp[m][n];
}
void sa(int n , int m)
{
	cur_arr = best_arr;
	curE = bestE;
	static uniform_int_distribution<int> dist(1, n);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (long double t = 60000; t >= 1e-8; t *= 0.99)
	{
		int x = dist(gen);
		int y = dist(gen);
		while (x == y) y = dist(gen);
		swap(cur_arr[x], cur_arr[y]);
		long double tmpE = cala(n, m, cur_arr);
		long double delta = tmpE - curE;
		if (delta < 0)
		{
			curE = tmpE;
			if (curE < bestE)
			{
				bestE = curE;
				best_arr = cur_arr;
			}
		}
		else if (exp(-delta / t) > prob(gen)) curE = tmpE;
		else swap(cur_arr[x], cur_arr[y]);
		if ((double)clock() / CLOCKS_PER_SEC > TL) break;
	}
	
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n, m;
	cin >> n >> m;
	init(n);
	for (int i = 1; i <= n; i++) cin >> best_arr[i];
	sort(best_arr.begin() + 1, best_arr.end());
	bestE = cala(n, m, best_arr);
	while ((double)clock() / CLOCKS_PER_SEC <= TL) sa(n, m);
	cout << fixed << setprecision(2) << sqrt(bestE / m);
	return 0;
}