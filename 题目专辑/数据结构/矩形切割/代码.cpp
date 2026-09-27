#include<bits/stdc++.h>
using namespace std;
int n, m, q, max_x, max_y;
vector<int> X, Y, op, k;
vector<unsigned long long> ans;
vector<int> a, b, a_sz, b_sz;
int find(int i,vector<int>& t)
{
	if (t[i] != i)
	{
		t[i] = find(t[i],t);
	}
	return t[i];
}
int unity(int i,vector<int>& t,vector<int>& sz)
{
	int a_i = find(i, t);
	int b_i = find(i + 1, t);
	sz[a_i] += sz[b_i];
	t[b_i] = a_i;
	return sz[a_i];
}
void init()
{
	X.reserve(q + 3);
	Y.reserve(q + 3);
	X.emplace_back(0);
	X.emplace_back(n);
	Y.emplace_back(0);
	Y.emplace_back(m);
	op.assign(q + 1, 0);
	k.assign(q + 1, 0);
	ans.assign(q + 1, 0);
}
void init_t(vector<int>& t,vector<int>& sz,vector<int>& xy,int& mmax)
{
	t.assign(xy.size() + 1, 0);
	sz.assign(xy.size() + 1, 0);
	for (int i = 1; i < xy.size(); i++)
	{
		t[i] = i;
		sz[i] = xy[i] - xy[i - 1];
		mmax = max(mmax, sz[i]);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n >> m >> q;
	init();
	for (int i = 1; i <= q; i++)
	{
		cin >> op[i] >> k[i];
		if (op[i] == 1) X.emplace_back(k[i]);
		else Y.emplace_back(k[i]);
	}
	sort(X.begin(), X.end());
	sort(Y.begin(), Y.end());
	init_t(a, a_sz, X, max_x);
	init_t(b, b_sz, Y, max_y);
	for (int i = q; i >= 1; --i)
	{
		ans[i] = (unsigned long long)max_x * max_y;
		if (op[i] == 1)
		{
			int idx = lower_bound(X.begin(), X.end(), k[i]) - X.begin();
			max_x = max(max_x, unity(idx, a, a_sz));
		}
		else
		{
			int idx = lower_bound(Y.begin(), Y.end(), k[i]) - Y.begin();
			max_y = max(max_y, unity(idx, b, b_sz));
		}
	}
	for (int i = 1; i <= q; i++) cout << ans[i] << '\n';
	return 0;
}
