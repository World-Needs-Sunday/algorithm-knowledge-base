#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, k;
	cin >> n >> k;
	cout << "YES\n";
	for (int i = 0; i < n; i++) cout << 3 * i << ' ';
	cout << '\n';
	for (int i = 0; i < n; i++) cout << i * 3 + 1 << ' ';

	return 0;
}
