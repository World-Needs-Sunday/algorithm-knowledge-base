#include<bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
inline int lowbit(int x) { return x & -x; }
struct treearr
{
	vector<int> len;
	vector<int> cnt;
	int sz, ans_len, ans_cnt;
	treearr(int n)
	{
		len.resize(n + 1, 0);
		cnt.resize(n + 1, 0);
		sz = n;
		ans_len = 0, ans_cnt = 0;
	}
	void add(int idx, int num, int t_cnt)
	{
		if (num > len[idx]) len[idx] = num, cnt[idx] = t_cnt;
		else if (num == len[idx]) cnt[idx] = (cnt[idx] + t_cnt) % mod;
	}
	void insert(int idx, int num, int t_cnt)
	{
		while (idx <= sz)
		{
			add(idx, num, t_cnt);
			idx += lowbit(idx);
		}
	}
	void get(int idx)
	{
		ans_len = 0, ans_cnt = 0;
		while (idx)
		{
			if (len[idx] > ans_len)
			{
				ans_len = len[idx];
				ans_cnt = cnt[idx];
			}
			else if (len[idx] == ans_len) ans_cnt = (ans_cnt + cnt[idx]) % mod;
			idx -= lowbit(idx);
		}
	}
};
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	vector<int> fac(n + 1);
	fac[0] = 1;
	for (int i = 1; i <= n; i++) fac[i] = (fac[i - 1] << 1) % mod;
	vector<int> A(n + 1);
	for (int i = 1; i <= n; i++) cin >> A[i];
	vector<int> tmp(A);
	sort(tmp.begin() + 1, tmp.end());
	tmp.erase(unique(tmp.begin() + 1, tmp.end()), tmp.end());
	treearr arr_a(tmp.size());//正序
	treearr arr_b(tmp.size());//倒序
	vector<int> len_a(n + 1),cnt_a(n + 1);
	vector<int> len_b(n + 1),cnt_b(n + 1);
	for (int i = n; i >= 1; --i)
	{
		A[i] = lower_bound(tmp.begin() + 1, tmp.end(), A[i]) - tmp.begin();
		int a = tmp.size() - A[i] + 1 , b = A[i];
		arr_a.get(a - 1);
		len_a[i] = arr_a.ans_len + 1;
		cnt_a[i] = arr_a.ans_cnt ? arr_a.ans_cnt : 1;
		arr_a.insert(a, len_a[i], cnt_a[i]);

		arr_b.get(b - 1);
		len_b[i] = arr_b.ans_len + 1;
		cnt_b[i] = arr_b.ans_cnt ? arr_b.ans_cnt : 1;
		arr_b.insert(b, len_b[i], cnt_b[i]);
	}
	int ans_len = 0, ans_cnt = 0;
	for (int i = 1; i <= n; i++)
	{
		int len = len_a[i] + len_b[i] - 1;
		int cnt = (((((long long)cnt_a[i] * cnt_b[i]) % mod) * fac[n - len]) % mod);
		if (len > ans_len) ans_cnt = cnt, ans_len = len;
		else if (len == ans_len) ans_cnt = (ans_cnt + cnt) % mod;
	}
	cout << ans_len << ' ' << ans_cnt;
	return 0;
}
