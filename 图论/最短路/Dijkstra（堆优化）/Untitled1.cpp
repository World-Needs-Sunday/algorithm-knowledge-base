#include<bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;
	int n,m,s;
	cin >> n >> m >> s;
	vector<int> A(n + 1,INF);
	vector<vector<pair<int,int>>> edges(n + 1);
	for(int i = 1;i <= m;i++)
	{
		int u,v,w;
		cin >> u >> v >> w;
		edges[u].emplace_back(v,w);
	}
	
	
	vector<bool> vis(n + 1,false);
	A[s] = 0;
	que.emplace(0,s);
	while(!que.empty())
	{
		pair<int,int> tmp = que.top();
		que.pop();
		if(vis[tmp.second]) continue;
		vis[tmp.second] = true;
		for(pair<int,int> i : edges[tmp.second])
		{
			if(vis[i.first]) continue;
			if(A[i.first] > tmp.first + i.second)
			{
				A[i.first] = tmp.first + i.second;
				que.emplace(A[i.first],i.first);
			}
		}
	}
	for(int i = 1;i <= n;i++) cout << A[i] << ' ';
	return 0;
} 
