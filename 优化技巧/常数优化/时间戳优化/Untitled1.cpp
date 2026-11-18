#include<bits/stdc++.h>
using namespace std;
unsigned int tim;
vector<unsigned int> vis;
void init(int n)
{
	vis.assign(n + 1, 0);
	tim = 0;
}

//时间戳优化
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cout << "请输入总的元素个数\n";
	int n, t;
	cin >> n;
	init(n);
	cout << "请输入查询的轮数\n";
	cin >> t;
	while (t--)
	{
		//每次不用重置vis数组，可以节省很多时间
		++tim;
		cout << "请输入本轮被标记的元素\n";
		int m;
		cin >> m;
		for (int i = 1; i <= m; i++)
		{
			int x;
			cin >> x;
			vis[x] = tim;
		}

		cout << "请输入查询\n";
		int q;
		cin >> q;
		for (int i = 1; i <= q; i++)
		{
			int x;
			cin >> x;
			if (vis[x] == tim) cout << "yes\n";
			else cout << "no\n";
		}
	}

	return 0;
}
