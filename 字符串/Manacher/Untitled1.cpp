#include<bits/stdc++.h>
using namespace std;
int manacher(const string& s)
{
	string t = "~#";
	t.reserve(s.size() * 2 + 2);
	for (char ch : s)
	{
		t += ch;
		t += '#';
	}

	int r = 0, mid = 0, ans = 0;
	vector<int> p(t.size(), 0);
	for (int i = 1; i < t.size(); i++)
	{
		if (i <= r) p[i] = min(p[mid * 2 - i], r - i + 1);
		while (t[i - p[i]] == t[i + p[i]]) ++p[i];
		if (p[i] + i > r)
		{
			r = p[i] + i - 1;
			mid = i;
		}
		ans = max(ans, p[i]);
	}
	return ans - 1;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	string s;
	cin >> s;
	cout << manacher(s);
	return 0;
}