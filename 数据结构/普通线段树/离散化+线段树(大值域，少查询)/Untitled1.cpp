#include<bits/stdc++.h>
/*洛谷P13825*/
using namespace std;
struct T
{
	int op, l, r, k;
};
inline unsigned long long getsum(int l, int r)
{
	return ((unsigned long long)l + r) * ((unsigned long long)r - l + 1) / 2;
}
vector<unsigned long long> tree;
vector<unsigned long long> lazy;
vector<T> A;
vector<int> myleft;
vector<int> myright;
inline int getleft(int i)
{
	return lower_bound(myleft.begin(), myleft.end(), i) - myleft.begin();
}
inline int getright(int i)
{
	return lower_bound(myright.begin(), myright.end(), i) - myright.begin();
}
int init(int n, int m)
{
	vector<int> dis;
	A.resize(m + 1);
	dis.emplace_back(1);
	dis.emplace_back(n);
	for (int i = 1; i <= m; i++)
	{
		cin >> A[i].op;
		if (A[i].op == 1)
		{
			cin >> A[i].l >> A[i].r >> A[i].k;
			dis.emplace_back(A[i].l);
			dis.emplace_back(A[i].r);
		}
		else
		{
			cin >> A[i].l >> A[i].r;
			dis.emplace_back(A[i].l);
			dis.emplace_back(A[i].r);
		}
	}
	sort(dis.begin(), dis.end());
	dis.erase(unique(dis.begin(), dis.end()), dis.end());

	int cnt = 0;
	myleft.emplace_back(0);
	myright.emplace_back(0);
	for (int i = 0; i < dis.size(); i++)
	{
		int j = i + 1;
		myleft.emplace_back(dis[i]);
		myright.emplace_back(dis[i]);
		if (j < dis.size() && dis[i] + 1 < dis[j])
		{
			myleft.emplace_back(dis[i] + 1);
			myright.emplace_back(dis[j] - 1);
			++cnt;
		}
		++cnt;
	}
	tree.resize(cnt * 4 + 1);
	lazy.resize(cnt * 4 + 1);
	return cnt;
}
void down(int i, int l, int r)
{
	if (lazy[i] == 0 || l == r) return;
	int mid = (l + r) >> 1;
	tree[i << 1] += lazy[i] * (myright[mid] - myleft[l] + 1);
	lazy[i << 1] += lazy[i];
	tree[(i << 1) | 1] += lazy[i] * (myright[r] - myleft[mid + 1] + 1);
	lazy[(i << 1) | 1] += lazy[i];
	lazy[i] = 0;
}
void insert(int rt, int l, int r, int ml, int mr, int k)
{
	down(rt, l, r);
	if (myleft[l] > mr || myright[r] < ml) return;
	if (ml <= myleft[l] && mr >= myright[r])
	{
		tree[rt] += (unsigned long long)k * (myright[r] - myleft[l] + 1);
		lazy[rt] += k;
		return;
	}
	int mid = (l + r) >> 1;
	insert(rt << 1, l, mid, ml, mr, k);
	insert((rt << 1) | 1, mid + 1, r, ml, mr, k);
	tree[rt] = tree[rt << 1] + tree[(rt << 1) | 1];
}
unsigned long long getval(int rt, int l, int r, int ml, int mr)
{
	down(rt, l, r);
	if (myleft[l] > mr || myright[r] < ml) return 0;
	if (ml <= myleft[l] && mr >= myright[r]) return tree[rt];
	int mid = (l + r) >> 1;
	return getval(rt << 1, l, mid, ml, mr) + getval((rt << 1) | 1, mid + 1, r, ml, mr);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	int sz = init(n, m);
	for (int i = 1; i <= m; i++)
	{
		if (A[i].op == 1) insert(1, 1, sz, A[i].l, A[i].r, A[i].k);
		else cout << getval(1, 1, sz, A[i].l, A[i].r) + getsum(A[i].l, A[i].r) << '\n';
	}
	return 0;
}
