#include<bits/stdc++.h>
using namespace std;
priority_queue<int> L;
priority_queue<int, vector<int>, greater<int>> R;
void my_move(int k)
{
	while (!L.empty() && L.size() > k)
	{
		int x = L.top();
		L.pop();
		R.emplace(x);
	}
	while (!R.empty() && L.size() < k)
	{
		int x = R.top();
		R.pop();
		L.emplace(x);
	}
}
void add(int x,int k)
{
	if (L.empty())
	{
		L.emplace(x);
		return;
	}
	if (x > L.top()) R.emplace(x);
	else L.emplace(x);
	my_move(k);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n,k,q;
	//n: 初始数据量 , k: 初始维护第k大 , q: 测试数量
	cin >> n >> k >> q;
	for (int i = 1; i <= n; i++)
	{
		int x;
		cin >> x;
		add(x, k);
	}
	cout << "当前第k小的元素是： " << L.top() << '\n';
	while (q--)
	{
		int op;
		cin >> op;
		if (op == 1) cout << "当前第k小的元素是： " << L.top() << '\n';
		else if (op == 2)
		{
			int x;
			cin >> x;
			add(x, k);
		}
		else if (op == 3)
		{
			cin >> k;
			my_move(k);
		}
	}
	return 0;
}
