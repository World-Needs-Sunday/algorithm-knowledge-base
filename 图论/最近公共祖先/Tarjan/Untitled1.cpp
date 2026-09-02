#include<bits/stdc++.h>
using namespace std;
int N, M;
vector<int> fa,ans,vis;
vector<vector<int>> A;
vector<vector<pair<int,int>>> arr;
int find(int i)
{
	if (fa[i] != i) fa[i] = find(fa[i]);
	return fa[i];
}
void tarjan(int rt)
{
	vis[rt] = true;
	for (int i : A[rt])
	{
		tarjan(i);
		fa[i] = rt;
	}
	for (int i = 0; i < arr[rt].size(); i++)
	{
		if (!vis[arr[rt][i].first]) continue;
		ans[arr[rt][i].second] = find(arr[rt][i].first);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> N >> M;
	A.resize(N + 1);
	vis.resize(N + 1,false);
	for (int i = 1; i <= N; i++)
	{
		int n;
		cin >> n;
		A[i].resize(n);
		for (int j = 0; j < n; j++)
		{
			cin >> A[i][j];
			vis[A[i][j]] = true;
		}
	}
	int rt = 1;
	while (vis[rt]) rt++;

	arr.resize(N + 1);
	for (int i = 1; i <= M; i++)
	{
		int u, v;
		cin >> u >> v;
		arr[u].emplace_back(v, i);
		arr[v].emplace_back(u, i);
	}

	fa.resize(N + 1);
	ans.resize(M + 1);
	for (int i = 1; i <= N; i++) fa[i] = i,vis[i] = false;
	tarjan(rt);

	for (int i = 1; i < ans.size();i++) cout << ans[i] << ' ';
	return 0;
}
