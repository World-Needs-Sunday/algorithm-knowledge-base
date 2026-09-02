#include<bits/stdc++.h>
using namespace std;
vector<long long> A, arr, lazy;
vector<vector<long long>> sum ;
vector<int> L, R;
int N, T, bk_sz, bk_cnt;
void init_bk(int id)
{

	for (int i = L[id]; i <= R[id]; i++)
	{
		A[i] += lazy[id];
		arr[i] = A[i];
	}
	sort(arr.begin() + L[id], arr.begin() + R[id] + 1);
	for (int i = L[id]; i <= R[id]; i++) sum[id][i - L[id] + 1] = sum[id][i - L[id]] + arr[i];
	lazy[id] = 0;
}
void init()
{
	bk_sz = ceil(sqrt(N));
	bk_cnt = (N + bk_sz - 1) / bk_sz;
	sum.resize(bk_cnt + 1, vector<long long>(bk_sz + 1, 0));
	arr.resize(N + 1);
	lazy.resize(bk_cnt + 1, 0);
	L.resize(bk_cnt + 1);
	R.resize(bk_cnt + 1);
	for (int i = 1; i <= bk_cnt; i++)
	{
		L[i] = (i - 1) * bk_sz + 1;
		R[i] = min(i * bk_sz, N);
		init_bk(i);
	}
}
inline long long get_2(int id , int l, int r, long long k)
{
	long long ans = 0;
	for (int i = l; i <= r; i++)
	{
		if (A[i] + lazy[id] <= k) ans += A[i] + lazy[id];
	}
	return ans;
}
inline long long get_2(int id, long long k)
{
	int i = upper_bound(arr.begin() + L[id], arr.begin() + R[id] + 1, k - lazy[id]) - (arr.begin() + L[id]);
	return sum[id][i] + (i * lazy[id]);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> N >> T;
	//初始化
	A.resize(N + 1);
	for (int i = 1; i <= N; i++) cin >> A[i];
	init();

	while (T--)
	{
		int op;
		cin >> op;
		if (op == 1)
		{
			int l, r;
			long long ans = 0;
			cin >> l >> r;
			int id_l = (l - 1) / bk_sz + 1, id_r = (r - 1) / bk_sz + 1;
			if (id_l == id_r)
			{
				for (int i = l; i <= r; i++) ans += lazy[id_l] + A[i];
			}
			else
			{
				for (int i = l; i <= R[id_l]; i++) ans += lazy[id_l] + A[i];
				for (int i = id_l + 1; i < id_r; i++) ans += sum[i][R[i] - L[i] + 1] + (lazy[i] * (R[i] - L[i] + 1));
				for (int i = L[id_r]; i <= r; i++) ans += lazy[id_r] + A[i];
			}
			cout << ans << '\n';
		}
		else if (op == 2)
		{
			int l, r;
			long long k, ans = 0;
			cin >> l >> r >> k;
			int id_l = (l - 1) / bk_sz + 1, id_r = (r - 1) / bk_sz + 1;
			
			if (id_l == id_r) ans += get_2(id_l, l, r, k);
			else
			{
				ans += get_2(id_l, l, R[id_l], k);
				for (int i = id_l + 1; i < id_r; i++) ans += get_2(i, k);
				ans += get_2(id_r, L[id_r], r, k);
			}
			cout << ans << '\n';
		}
		else if (op == 3)
		{
			int l, r;
			long long w; 
			cin >> l >> r >> w;
			int id_l = (l - 1) / bk_sz + 1, id_r = (r - 1) / bk_sz + 1;
			if (id_l == id_r)
			{
				for (int i = l; i <= r; i++) A[i] += w;
				init_bk(id_l);
			}
			else
			{
				//左散块
				for (int i = l; i <= R[id_l]; i++) A[i] += w;
				init_bk(id_l);

				//中间整块
				for (int i = id_l + 1; i < id_r; i++) lazy[i] += w;

				//右散块
				for (int i = L[id_r]; i <= r; i++) A[i] += w;
				init_bk(id_r);
			}
		}
		
	}

	return 0;
}
