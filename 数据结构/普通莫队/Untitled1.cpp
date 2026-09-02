#include<algorithm>
#include<iostream>
#include<vector>
#include<cmath>
/*洛谷P2709*/
using namespace std;
int n, m, k, s;
long long cur = 0;
vector<int> cnt;
vector<int> A;
vector<long long> ans;
struct query
{
	int l, r, idx;
	bool operator< (query& b)
	{
		int al = this->l / s;
		int bl = b.l / s;
		if (al != bl) return al < bl;
		else
		{
			if (al & 1) return this->r > b.r;
			else return this->r < b.r;
		}
	}
};
vector<query> B;



void my_push(int i)
{
	cur -= (long long)cnt[A[i]] * cnt[A[i]];
	cnt[A[i]]++;
	cur += (long long)cnt[A[i]] * cnt[A[i]];
}
void my_pop(int i)
{
	cur -= (long long)cnt[A[i]] * cnt[A[i]];
	cnt[A[i]]--;
	cur += (long long)cnt[A[i]] * cnt[A[i]];
}

int main()
{
	cin >> n >> m >> k;
	s = sqrt(n) + 1;
	A.resize(n + 1, 0);
	B.resize(m + 1);
	ans.resize(m + 1, 0);
	cnt.resize(k + 1, 0);
	for (int i = 1;i <= n;i++) cin >> A[i];
	for (int i = 1;i <= m;i++)
	{
		cin >> B[i].l >> B[i].r;
		B[i].idx = i;
	}
	sort(B.begin() + 1, B.end());
	
	int l = 1;int r = 0;
	for (int i = 1;i <= m;i++)
	{
		while (B[i].l < l) my_push(--l);
		while (B[i].r > r) my_push(++r);
		while (B[i].l > l) my_pop(l++);
		while (B[i].r < r) my_pop(r--);
		ans[B[i].idx] = cur;
	}
	for (int i = 1;i <= m;i++) cout << ans[i] << endl;
	return 0;
}

