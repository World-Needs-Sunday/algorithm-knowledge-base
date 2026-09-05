#include<bits/stdc++.h>
using namespace std;
vector<int> sunday_next, ans;
void init(const string& str)
{
	int m = str.size();
	sunday_next.assign(128, m + 1);
	for (int i = 0; i < m; i++) sunday_next[str[i]] = m - i;
}

void sunday(const string& a, const string& b)
{
	ans.clear();
	int n = a.size(), m = b.size();
	for (int i = 0; i <= n - m; i += sunday_next[a[i + m]])
	{
		int j = 0;
		while (j < m && a[i + j] == b[j]) j++;
		if (j == m) ans.emplace_back(i);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--)
	{
		string p;
		int t;
		cin >> p >> t;
		init(p);
		while (t--)
		{
			ans.clear();
			string s;
			cin >> s;
			sunday(s, p);
			if (ans.empty()) cout << -1;
			else for (int i : ans) cout << i << ' ';
			cout << '\n';
		}
	}
	return 0;
}
