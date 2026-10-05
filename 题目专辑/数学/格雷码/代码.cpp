#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	unsigned long long n, k, t = 0;
	cin >> n >> k;
	for (int i = n - 1; i >= 0; i--)
	{
		unsigned long long j = ((unsigned long long)1 << i);
		if (k & j)
		{
			cout << (t ^ 1);
			t = 1;
		}
		else
		{
			cout << t;
			t = 0;
		}
	}
	return 0;
}
