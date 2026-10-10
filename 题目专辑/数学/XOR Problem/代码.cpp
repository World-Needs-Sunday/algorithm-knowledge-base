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
		int n;
		cin >> n;
		int s = 1;
		while (s <= n) s <<= 1;
		int len = 2 * s - 1;
		vector<int> p(2 * s);
		for (int i = 0; i < s; i++) p[i] = i ^ (i >> 1);
		for (int i = 0; i < s; ++i) p[s + i] = p[s - 1 - i];
		cout << len << '\n';
		for (int i = 1; i < 2 * s; ++i) cout << (p[i - 1] ^ p[i]) << ' ';
		cout << '\n';
	}
	return 0;
}