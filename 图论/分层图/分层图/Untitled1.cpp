#include<bits/stdc++.h>
using namespace std;
vector<vector<pair<int,int>>> edges;
struct node
{
	long long w;
	int dist,k;
	node(){}
	node(long long a,int b,int c)
	{
		w = a;
		dist = b;
		k = c;
	}
};
struct cmp
{
	bool operator() (node& a,node& b)
	{
		if(a.w == b.w) return a.k < b.k;
		else return a.w > b.w;
	}
};
void init(int n)
{
	edges.assign(n + 1,vector<pair<int,int>>());
}

long long dij(int s,int t,int n,int k)
{
	priority_queue<node,vector<node>,cmp> que;
	vector<int> vis(n + 1,-1);
	que.emplace(0,s,k);
	while(!que.empty())
	{
		node tmp = que.top();
		que.pop();
		if(vis[tmp.dist] >= tmp.k) continue;
		vis[tmp.dist] = tmp.k;
		if(tmp.dist == t) return tmp.w;
		
		if(tmp.k >= 1)
		{
			for(pair<int,int>& p : edges[tmp.dist])
			{
				if(vis[p.first] >= tmp.k - 1) continue;
				que.emplace(tmp.w , p.first , tmp.k - 1);
			}
		}
		
		for(pair<int,int>& p : edges[tmp.dist])
		{
			if(vis[p.first] >= tmp.k) continue;
			que.emplace(tmp.w + p.second , p.first , tmp.k);
		}
	}
	return -1;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,m,k,s,t;
	cin >> n >> m >> k;
	init(n);
	cin >> s >> t;
	for(int i = 1;i <= m;i++)
	{
		int a,b,c;
		cin >> a >> b >> c;
		edges[a].emplace_back(b,c);
		edges[b].emplace_back(a,c);
	}
	
	cout << dij(s,t,n,k);
	return 0;
}
