#include<bits/stdc++.h>
using namespace std;
const long long INF = INT_MAX;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,m,s;
	cin >> n >> m >> s;
	vector<vector<pair<int,int>>> edges(n + 1);
	for(int i = 1;i <= m;i++)
	{
		int u,v,w;
		cin >> u >> v >> w;
		edges[u].emplace_back(v,w);
	}
	
	vector<long long> A(n + 1,INF);
	vector<int> que(n + 1,0),tmp(n + 1,0); 
	vector<bool> vis(n + 1,false);
	int idx = 0,id = 0;
	A[s] = 0;
	que[idx++] = s;
	for(int t = 1;t < n;++t)
	{
		while(idx)
		{
			for(pair<int,int> i : edges[que[idx - 1]])
			{
				if(A[i.first] > A[que[idx - 1]] + i.second)
				{
					A[i.first] = A[que[idx - 1]] + i.second;
					if(!vis[i.first])
					{
						vis[i.first] = true;
					 	tmp[id++] = i.first;
					}
				}
			}
			--idx;
		}
		if(!id) break; 
		while(id)
		{
			vis[tmp[id - 1]] = false;
			que[idx++] = tmp[id - 1];
			--id; 
		}
	}
	
	for(int i = 1;i <= n;i++) cout << A[i] << ' ';
	return 0;
} 


