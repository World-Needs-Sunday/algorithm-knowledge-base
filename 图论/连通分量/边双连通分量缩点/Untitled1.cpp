#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges, ans, e_DCC_edges;
vector<int> dfn, low, sk, col;
int tim, top, col_cnt;
void init(int n, int m)
{
	dfn.assign(n + 1, 0);
	low.assign(n + 1, 0);
	edges.assign(n + 1, vector<int>());
	sk.assign(n + 1, 0);
	col.assign(n + 1, 0);
	ans.assign(1, vector<int>());
	tim = 0;
	top = 0;
	col_cnt = 0;
}
void add(int u, int v)
{
	edges[u].emplace_back(v);
	edges[v].emplace_back(u);
}
inline void mark(int rt)
{
	dfn[rt] = low[rt] = ++tim;
	sk[top++] = rt;
}
void tarjan(int rt, int fa)
{
	int cnt = 0;
	mark(rt);
	for (int i : edges[rt])
	{
		if (fa == i && !cnt)
		{
			++cnt;
			continue;
		}
		if (!dfn[i])
		{
			tarjan(i, rt);
			low[rt] = min(low[i], low[rt]);
			if (low[i] > dfn[rt])
			{
				++col_cnt;
				ans.emplace_back();
				while (sk[top] != i)
				{
					col[sk[--top]] = col_cnt;
					ans[col_cnt].emplace_back(sk[top]);
				}
			}
		}
		else low[rt] = min(dfn[i], low[rt]);
	}
}

void e_DCC(int n, int m)
{
	for (int i = 1; i <= n; i++)
	{
		if (!dfn[i]) tarjan(i, -1);
		if (top)
		{
			++col_cnt;
			ans.emplace_back();
			while (top)
			{
				col[sk[--top]] = col_cnt;
				ans[col_cnt].emplace_back(sk[top]);
			}
		}
	}
	
	e_DCC_edges.assign(col_cnt + 1, vector<int>());
	vector<int> vis(col_cnt + 1, 0);
	for (int u = 1; u <= col_cnt; u++)
	{
		for (int i : ans[u])
		{
			for (int j : edges[i])
			{
				int v = col[j];
				if (vis[v] == u || u == v) continue;
				vis[v] = u;
				e_DCC_edges[u].emplace_back(v);
			}
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	init(n, m);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		add(u, v);
	}
	e_DCC(n, m);
	cout << "e_DCC缩点后有 " << col_cnt << " 个边双连通分量" << '\n';
	for (int i = 1; i <= col_cnt; i++)
	{
		cout << "超级点" << i << "含有: ";
		for (int j : ans[i]) cout << j << ' ';
		cout << '\n';

		cout << "超级点" << i << "有" << e_DCC_edges[i].size() << "个点连接\n";
		for (int j : e_DCC_edges[i]) cout << j << ' ';
		cout << '\n';
	}
	return 0;
}
