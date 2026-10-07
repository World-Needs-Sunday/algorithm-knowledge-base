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
		int n, m, x, y;
		cin >> n >> m >> x >> y;
		unordered_set<int> vis;
		vector<int> arr_x(x + 1,0);
		vector<int> arr_y(y + 1, 0);
		for (int i = 1; i <= x; i++) cin >> arr_x[i];
		for (int i = 1; i <= y; i++) cin >> arr_y[i];
		int idx_x = x,cnt_x = 0;
		int idx_y = y,cnt_y = 0;
		int tmp = 0;
		long long sum = 0;
		while (cnt_x + cnt_y + tmp < n + m - 1 && cnt_x < n && cnt_y < m && idx_x > 0 && idx_y > 0)
		{
			while (idx_x > 0 && vis.count(arr_x[idx_x]))idx_x--;
			while (idx_y > 0 && vis.count(arr_y[idx_y]))idx_y--;
			if (idx_x == 0 || idx_y == 0) break;
			if (arr_x[idx_x] == arr_y[idx_y])
			{
				sum += arr_x[idx_x];
				vis.insert(arr_x[idx_x]);
				--idx_x;
				--idx_y;
				++tmp;
			}
			else if(arr_x[idx_x] >= arr_y[idx_y])
			{
				sum += arr_x[idx_x];
				vis.insert(arr_x[idx_x]);
				--idx_x;
				++cnt_x;
			}
			else
			{
				sum += arr_y[idx_y];
				vis.insert(arr_y[idx_y]);
				--idx_y;
				++cnt_y;
			}
		}

		if (idx_x == 0 && cnt_x < n)
		{
			int t = min(n - cnt_x,tmp);
			tmp -= t;
			cnt_x += t;
		}

		if (idx_y == 0 && cnt_y < m)
		{
			int t = min(m - cnt_y , tmp);
			tmp -= t;
			cnt_y += t;
		}

		while (idx_x > 0 && tmp + cnt_x + cnt_y < n + m - 1 && tmp + cnt_x < n)
		{
			if (!vis.count(arr_x[idx_x]))
			{
				vis.insert(arr_x[idx_x]);
				sum += arr_x[idx_x];
				++cnt_x;
			}
			--idx_x;
		}

		while (idx_y > 0 && tmp + cnt_x + cnt_y < n + m - 1 && tmp + cnt_y < m)
		{
			if (!vis.count(arr_y[idx_y]))
			{
				vis.insert(arr_y[idx_y]);
				sum += arr_y[idx_y];
				++cnt_y;
			}
			--idx_y;
		}

		cout << sum << '\n';
	}
	return 0;
}
