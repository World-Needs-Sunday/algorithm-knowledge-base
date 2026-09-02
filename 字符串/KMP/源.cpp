#include<bits/stdc++.h>
using namespace std;
vector<int> kmp_next,ans;
void init(const string& str)
{
	kmp_next.clear();
	int m = str.size(),j = -1;
	kmp_next.resize(m,-1);
	for (int i = 1; i < m; i++)
	{
		while (j != -1 && str[i] != str[j + 1]) j = kmp_next[j];
		if (str[i] == str[j + 1]) j++;
		kmp_next[i] = j;
	}
}

void kmp(const string& a, const string& b)
{
	int n = a.size(), m = b.size();
	int j = -1;
	for (int i = 0; i < n; i++)
	{
		while (j != -1 && a[i] != b[j + 1]) j = kmp_next[j];
		if (a[i] == b[j + 1]) j++;
		if (j == m - 1)
		{
			ans.emplace_back(i - m + 1);
			j = kmp_next[j];
		}
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
			kmp(s, p);
			if (ans.empty()) cout << -1;
			else for (int i : ans) cout << i << ' ';
			cout << '\n';
		}
	}
	return 0;
}
