#include<bits/stdc++.h>
using namespace std;
vector<unsigned int> dfn,low;
vector<int> sk,vis;
vector<bool> in_sk;
vector<vector<int>> edges;
unsigned int tim,top_idx,scc_cnt;
void init(int n)
{
	edges.assign(2 * n + 1,vector<int>());
	dfn.assign(2 * n + 1,0);
	low.assign(2 * n + 1,0);
	in_sk.assign(2 * n + 1,false);
	sk.assign(2 * n + 1,0);
	vis.assign(2 * n + 1,-1);
	tim = 0;
	top_idx = 0;
	scc_cnt = 0;
}

inline void mark(int idx)
{
	dfn[idx] = low[idx] = ++tim;
	sk[++top_idx] = idx;
	in_sk[idx] = true;
}

void tarjan(int idx)
{
	mark(idx);
	for(int to : edges[idx])
	{
		if(!dfn[to])
		{
			tarjan(to);
			low[idx] = min(low[idx],low[to]);
		}
		else if(in_sk[to]) low[idx] = min(low[idx],dfn[to]);
	}
	
	if(dfn[idx] == low[idx])
	{
		++scc_cnt;
		while(in_sk[idx])
		{
			int id = sk[top_idx--];
			in_sk[id] = false;
			vis[id] = scc_cnt;
		}
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	
	int n,m;
	cin >> n >> m;
	init(n);
	//0在【1，n】 1在【n + 1,2n】;
	for(int d = 1;d <= m;d++)
	{
		int i,a,j,b;
		cin >> i >> a >> j >> b;
		// i != a -> j = b
		edges[i + (a ^ 1) * n].emplace_back(j + b * n);
		
		// j != b -> i = a;
		edges[j + (b ^ 1) * n].emplace_back(i + a * n);
	}
	for(int i = 1;i <= 2 * n;i++)
	{
		if(!dfn[i]) tarjan(i);
	}
	
	for(int i = 1;i <= n;i++)
	{
		if(vis[i] == vis[i + n])
		{
			cout << "IMPOSSIBLE\n";
			return 0;
		}
	}
	
	cout << "POSSIBLE\n";
	for(int i = 1;i <= n;i++)
	{
		if(vis[i] < vis[i + n]) cout << 0 << ' ';
		else cout << 1 << ' ';
	}
	return 0;
}
