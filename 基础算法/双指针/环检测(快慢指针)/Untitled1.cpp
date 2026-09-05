#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--)
	{
		//n是节点总数，s是入口，保证n-1个输入并且组成一个链子，判断是否带环
		int n, s;
		cin >> n >> s;
		vector<int> to(n + 1, 0);
		for (int i = 1;i < n;i++)
		{
			int u, v;
			cin >> u >> v;
			to[u] = v;
		}

		int fast = s, slow = s;
		do
		{
			slow = to[slow];
			fast = to[to[fast]];
		}
		while (fast != 0 && slow != fast);
		if (fast == 0) cout << "无环" << '\n';
		else
		{
			fast = s;
			while (fast != slow)
			{
				fast = to[fast];
				slow = to[slow];
			}
			cout << "有环，环入口为：" << fast << '\n';
		}
	}

	return 0;
}
