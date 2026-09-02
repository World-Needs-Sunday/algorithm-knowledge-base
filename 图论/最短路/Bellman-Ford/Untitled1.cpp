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
	A[s] = 0;
	for(int t = 1;t < n;t++)
	{
		bool tmp = true;
		for(int i = 1;i <= n;i++)
		{
			for(pair<int,int> j : edges[i])
			{
				if(A[j.first] > A[i] + j.second) 
				{
					A[j.first] = A[i] + j.second;
					tmp = false;
				}
			}
		}
		if(tmp) break;
	}
	
	for(int i = 1;i <= n;i++) cout << A[i] << ' ';
	return 0;
} 

