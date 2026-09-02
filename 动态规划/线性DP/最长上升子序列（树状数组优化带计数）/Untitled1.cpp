#include<bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
inline int lowbit(int x) { return x & -x; }
struct treearr
{
	vector<int> len;
	vector<int> cnt;
	int sz,ans_len,ans_cnt;
	treearr(int n)
	{
		len.resize(n + 1, 0);
		cnt.resize(n + 1, 0);
		sz = n;
		ans_len = 0, ans_cnt = 0;
	}
	void add(int idx,int num,int t_cnt)
	{
		if (num > len[idx]) len[idx] = num,cnt[idx] = t_cnt;
		else if (num == len[idx]) cnt[idx] = (cnt[idx] + t_cnt) % mod;
	}
	void insert(int idx,int num,int t_cnt)
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
	int T;
	cin >> T;
	while (T--)
	{
		int n,sz;
		cin >> n;
		vector<int> A(n + 1);
		vector<int> tmp(n + 1);
		for (int i = 1; i <= n; i++)
		{
			cin >> A[i];
			tmp[i] = A[i];
		}
		sort(tmp.begin() + 1, tmp.end());
		tmp.erase(unique(tmp.begin() + 1, tmp.end()), tmp.end());
		treearr arr(tmp.size());
		for (int i = 1; i <= n; i++) 
		{
			A[i] = lower_bound(tmp.begin() + 1, tmp.end(), A[i]) - tmp.begin();
			arr.get(A[i] - 1);
			int new_len = arr.ans_len + 1;
			int new_cnt = arr.ans_cnt ? arr.ans_cnt : 1;
			arr.insert(A[i], new_len, new_cnt);
		}
		arr.get(tmp.size());
		cout << arr.ans_len << ' ' << arr.ans_cnt << '\n';
	}
	return 0;
}
