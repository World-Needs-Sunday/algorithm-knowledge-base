#include<bits/stdc++.h>
using namespace std;
vector<vector<pair<int,long long>>> A;
int N,M;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	cin >> N >> M;
	A.resize(N + 1);
	for(int i = 1;i <= M;i++)
	{
		int u,v;
		long long w;
		cin >> u >> v >> w;
		A[u].emplace_back(v,w);
		A[v].emplace_back(u,w);
	}

	vector<bool> vis(N + 1,false);
	priority_queue<pair<long long , int>,vector<pair<long long,int>>,greater<pair<long long , int>>> que;
	que.emplace(0,1);
	long long ans = 0;
	int cnt = 0;
	while(!que.empty())
	{
		int nd = que.top().second;
		long long w = que.top().first;
		que.pop();
		if(vis[nd]) continue;

		vis[nd] = true;
		ans += w , ++cnt;
		if(cnt == N) break;
		for(int i = 0;i < A[nd].size();i++)
		{
			int nd_son = A[nd][i].first;
			if(vis[nd_son]) continue;
			que.emplace(A[nd][i].second,nd_son);
		}
	}

	if(cnt == N) cout << ans;
	else cout << "orz";
	return 0;
}
