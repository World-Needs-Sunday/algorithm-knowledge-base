#include<bits/stdc++.h>
using namespace std;
const double TL = 0.9;
vector<int> cur_arr, best_arr;//cur_arr[i]表示 奶牛i所在的位置
vector<vector<int>> edges;
long long curE = 0, bestE = 0;
mt19937 gen;
void init(int n)
{
	cur_arr.assign(n + 1, 0);
	best_arr.assign(n + 1, 0);
	for (int i = 1; i <= n; i++) best_arr[i] = i;
	edges.assign(n + 1, vector<int>());
	curE = 0, bestE = 0;
	gen.seed(time(nullptr));
}
long long cala(int a, int b, vector<int>& idx)
{
	long long ans = curE;
	for (int i : edges[a])
	{
		if (i != b)
		{
			ans -= abs(idx[a] - idx[i]);
			ans += abs(idx[b] - idx[i]);
		}
	}
	for (int i : edges[b])
	{
		if (i != a)
		{
			ans -= abs(idx[b] - idx[i]);
			ans += abs(idx[a] - idx[i]);
		}
	}
	return ans;
}
void sa(int n)
{
	cur_arr = best_arr;
	curE = bestE;
	static uniform_int_distribution<int> dist(1, n);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (long double t = 20000; t >= 1e-8; t *= 0.99)
	{
		int x = dist(gen);
		int y = dist(gen);
		while(x == y) y = dist(gen);
		long long tmpE = cala(x, y, cur_arr);
		long long delta = tmpE - curE;
		if (delta < 0 || exp((long double)-delta / t) > prob(gen))
		{
			curE = tmpE;
			swap(cur_arr[x], cur_arr[y]);
		}
		if ((double)clock() / CLOCKS_PER_SEC > TL) break;
	}
	if (curE < bestE)
	{
		bestE = curE;
		best_arr = cur_arr;
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			int x;
			cin >> x;
			if (i < x) bestE += abs(i - x);
			edges[i].emplace_back(x);
		}
	}
	while ((double)clock() / CLOCKS_PER_SEC <= TL) sa(n);
	cout << bestE;

	return 0;

}