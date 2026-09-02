#include<bits/stdc++.h>
using namespace std;
/*U436983*/
int N , V , root;
vector<pair<int,int>> A;
vector<vector<int>> son;
vector<vector<int>> dp;
void dfs(int idx)
{
	for(int i = A[idx].first;i <= V;i++) dp[idx][i] = A[idx].second;
	for(int i : son[idx])
	{
		dfs(i);
		for(int j = V ; j > A[idx].first ; j--)
		{
			for(int k = 1;j - k >= A[idx].first;k++)
			{
				dp[idx][j] = max(dp[idx][j - k] + dp[i][k] , dp[idx][j]);
			}
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	
	cin >> N >> V;
	dp.resize(N + 1,vector<int>(V + 1,0));
	A.resize(N + 1);
	son.resize(N + 1,vector<int>());
	for(int i = 1;i <= N;i++)
	{
		int p;
		cin >> A[i].first >> A[i].second >> p;
		if(p == -1) root = i;
		else son[p].emplace_back(i);
	}
	dfs(root);
	cout << dp[root][V];
	return 0;	
} 
