#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges,ans,scc_edges;
vector<int> sk,dns,low,col,scc_in,scc_out;
vector<bool> in_sk;
int idx, tim, cnt;
inline void init(int n)
{
	idx = 0, tim = 1, cnt = 0;
	sk.assign(n + 1,0);
	in_sk.assign(n + 1,false);
	dns.assign(n + 1,0);
	low.assign(n + 1,0);
	col.assign(n + 1,0);
	ans.assign(1, {});
}
inline void mark(int rt)
{
	dns[rt] = tim;
	low[rt] = tim;
	sk[idx++] = rt;
	in_sk[rt] = true;
	tim++;
}
void tarjan(int rt)
{
	mark(rt);
	for (int i : edges[rt])
	{
		if (!dns[i])
		{
			tarjan(i);
			low[rt] = min(low[rt], low[i]);
		}
		else if (in_sk[i]) low[rt] = min(low[rt], dns[i]);
	}
	if (dns[rt] == low[rt])
	{
		++cnt;
		ans.emplace_back();
		while (in_sk[rt])
		{
			col[sk[idx - 1]] = cnt;
			ans[cnt].emplace_back(sk[idx - 1]);
			in_sk[sk[idx - 1]] = false;
			--idx;
		}
	}
}

void SCC(int n)
{
	init(n);
	for (int i = 1; i <= n; i++)
	{
		if (!dns[i]) tarjan(i);
	}

	scc_edges.assign(cnt + 1, vector<int>());
	scc_in.assign(cnt + 1, 0);
	scc_out.assign(cnt + 1, 0);
	vector<int> vis(cnt + 1, false);
	for (int u = 1; u <= cnt; u++)
	{
		for (int i : ans[u])
		{
			for (int j : edges[i])
			{
				int v = col[j];
				if (vis[v] == u || u == v) continue;
				scc_edges[u].emplace_back(v);
				vis[v] = u;
				scc_out[u]++;
				scc_in[v]++;
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
	edges.assign(n + 1, {});
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		edges[u].emplace_back(v);
	}
	SCC(n);
	cout << "SCC缩点和有 " << cnt << " 个强联通分量" << '\n';
	for (int i = 1; i <= cnt; i++)
	{
		cout << "超级点" << i << "含有: ";
		for (int j : ans[i]) cout << j << ' ';
		cout << '\n';

		cout << "超级点" << i << "有" << scc_in[i] << "个入度, " << scc_out[i] << "个出度" << '\n';
		for (int j : scc_edges[i])
		{
			cout << i << "->" << j << '\n';
		}
	}
	return 0;
}