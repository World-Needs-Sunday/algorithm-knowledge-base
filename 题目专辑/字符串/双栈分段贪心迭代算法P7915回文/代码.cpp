#include<bits/stdc++.h>
using namespace std;
vector<int> a;
vector<char> s;
int n;
void init()
{
	a.clear();
	s.clear();
	a.resize(2 * n + 5);
	s.resize(2 * n + 5);
}

bool fn_L()
{
	int k = 2;
	s[1] = 'L', s[2 * n] = 'L';
	while (a[1] != a[k]) ++k;
	int ctop = 2, cback = k - 1, dtop = 2 * n, dback = k + 1;
	int idx = 2;
	while (ctop <= cback || dtop >= dback)
	{
		if ((ctop <= cback) && ((a[ctop] == a[cback] && ctop != cback) || (a[ctop] == a[dback] && dtop >= dback)))
		{
			s[idx] = 'L';
			if (a[ctop] == a[cback] && ctop != cback)
			{
				s[2 * n - idx + 1] = 'L';
				--cback;
			}
			else
			{
				s[2 * n - idx + 1] = 'R';
				++dback;
			}
			++ctop;
		}
		else if ((dtop >= dback) && ((a[dtop] == a[cback] && ctop <= cback) || (a[dtop] == a[dback] && dtop != dback)))
		{
			s[idx] = 'R';
			if (a[dtop] == a[cback] && ctop <= cback)
			{
				s[2 * n - idx + 1] = 'L';
				--cback;
			}
			else
			{
				s[2 * n - idx + 1] = 'R';
				++dback;
			}
			--dtop;
		}
		else return false;
		++idx;
	}
	return true;
}


bool fn_R()
{
	int k = 1;
	s[1] = 'R', s[2 * n] = 'L';
	while (a[2 * n] != a[k]) ++k;
	int ctop = 1, cback = k - 1, dtop = 2 * n - 1, dback = k + 1;
	int idx = 2;
	while (ctop <= cback || dtop >= dback)
	{
		if ((ctop <= cback) && ((a[ctop] == a[cback] && ctop != cback) || (a[ctop] == a[dback] && dtop >= dback)))
		{
			s[idx] = 'L';
			if (a[ctop] == a[cback] && ctop != cback)
			{
				s[2 * n - idx + 1] = 'L';
				--cback;
			}
			else
			{
				s[2 * n - idx + 1] = 'R';
				++dback;
			}
			++ctop;
		}
		else if ((dtop >= dback) && ((a[dtop] == a[cback] && ctop <= cback) || (a[dtop] == a[dback] && dtop != dback)))
		{
			s[idx] = 'R';
			if (a[dtop] == a[cback] && ctop <= cback)
			{
				s[2 * n - idx + 1] = 'L';
				--cback;
			}
			else
			{
				s[2 * n - idx + 1] = 'R';
				++dback;
			}
			--dtop;
		}
		else return false;
		++idx;
	}
	return true;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--)
	{
		cin >> n;
		init();
		for (int i = 1; i <= 2 * n; i++) cin >> a[i];
		if (fn_L())
		{
			for (int i = 1; i <= 2 * n; i++) cout << s[i];
			cout << '\n';
		}
		else if (fn_R())
		{
			for (int i = 1; i <= 2 * n; i++) cout << s[i];
			cout << '\n';
		}
		else cout << -1 << '\n';
	}
	return 0;
}
