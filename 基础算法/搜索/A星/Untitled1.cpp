#include<bits/stdc++.h>
using namespace std;
vector<vector<pair<int, int>>> edges,re_edges;
vector<long long> g;
vector<unsigned int> cnt;
void init(int n)
{
	edges.assign(n + 1, {});
	re_edges.assign(n + 1, {});
	g.assign(n + 1, LLONG_MAX >> 2);
	cnt.assign(n + 1, 0);
}
void dij(int n, int s, vector<vector<pair<int, int>>>& e, vector<long long>& arr)
{
	vector<bool> vis(n + 1, false);
	priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> que;
	que.emplace(0, s);
	arr[s] = 0;
	while (!que.empty())
	{
		long long w = que.top().first;
		int d = que.top().second;
		que.pop();
		if (vis[d]) continue;
		vis[d] = true;
		for (pair<int, int>& i : e[d])
		{
			int to = i.first;
			int to_w = i.second;
			if (vis[to]) continue;
			if (arr[to] > w + to_w)
			{
				arr[to] = w + to_w;
				que.emplace(arr[to], to);
			}
		}
	}
}

long long A(int s,int t, int k,vector<vector<pair<int, int>>>& e, vector<long long>& g)
{
	priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> que;
	que.emplace(g[s], s);
	while (!que.empty())
	{
		long long w = que.top().first;
		int d = que.top().second;
		que.pop();
		if (cnt[d] >= k) continue;
		++cnt[d];
		if (cnt[d] == k && d == t) return w;
		for (pair<int, int>& i : e[d])
		{
			int to = i.first;
			int to_w = i.second;
			if (cnt[to] >= k) continue;
			que.emplace(w - g[d] + to_w + g[to] , to);
		}
	}
	return -1;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int N, M;
	cin >> N >> M;
	init(N);
	for (int i = 1; i <= M; i++)
	{
		int a, b, w;
		cin >> a >> b >> w;
		edges[a].emplace_back(b, w);
		re_edges[b].emplace_back(a, w);
	}
	int S, T, K;
	cin >> S >> T >> K;
	if (S == T) ++K;
	dij(N, T, re_edges, g);
	cout << A(S, T, K, edges, g);
	return 0;
}