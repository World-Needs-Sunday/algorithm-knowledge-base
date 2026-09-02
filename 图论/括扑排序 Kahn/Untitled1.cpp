#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges;
int n,m;
int main()
{
	cin >> n >> m;
	edges.resize(n + 1); 
	vector<int> q(n + 1 , 0);
	vector<int> ans;
	int cnt = n;
	for(int i = 0;i < m;i++)
	{
		int a,b;
		cin >> a >> b;
		edges[a].emplace_back(b);
		q[b]++;
	}
	
	queue<int> que;
	for(int i = 1;i <= n;i++)
	{
		if(q[i] == 0)
		{
			que.emplace(i);
			ans.emplace_back(i);
			cnt--;
		}
	}
	
	while(!que.empty())
	{
		int i = que.front();
		que.pop();
		for(int idx : edges[i])
		{
			q[idx]--;
			if(q[idx] == 0)
			{
				que.emplace(idx);
				ans.emplace_back(idx);
				cnt--;
			}
		}
	}
	
	if(cnt) cout << "有环" << endl;
	else 
	{
		for(int i = 0;i < n;i++) cout << ans[i] << " ";
	}
	
	return 0;
} 
