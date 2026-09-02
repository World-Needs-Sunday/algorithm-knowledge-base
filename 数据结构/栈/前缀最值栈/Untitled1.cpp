#include<bits/stdc++.h>
using namespace std;
const int nmax = 1e5;
int sk_num[nmax], sk_big[nmax];
int id = 0;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int op;
		cin >> op;
		if (op == 1)
		{
			int x;
			cin >> x;
			sk_num[id] = x;
			if (id > 0) sk_big[id] = max(sk_big[id - 1], x);
			else sk_big[id] = x;
			++id;
		}
		else if (op == 2) --id;
		else if (op == 3)
		{
			cout << "当前栈顶元素为: " << sk_num[id - 1] << '\n';
			cout << "栈内最大元素为: " << sk_big[id - 1] << '\n';
		}
	}
	return 0;
}
