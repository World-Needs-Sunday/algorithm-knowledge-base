#include<bits/stdc++.h>
using namespace std;
/*洛谷P1352*/
vector<int> A;
vector<vector<int>> dp;
vector<bool> fa;
vector<vector<int>> son;
void dfs(int idx)
{
	dp[idx][1] += A[idx];
	for(int i : son[idx])
	{
		dfs(i);
		dp[idx][1] += dp[i][0];
		dp[idx][0] += max(dp[i][0] , dp[i][1]);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,root = 1;
	cin >> n;
	A.resize(n + 1,0);
	fa.resize(n + 1,false);
	son.resize(n + 1,vector<int>());
	dp.resize(n + 1,vector<int>(2,0));
	for(int i = 1;i <= n;i++) cin >> A[i];
	for(int i = 1;i < n;i++)
	{
		int l,k;
		cin >> l >> k;
		son[k].emplace_back(l);
		fa[l] = true;
	}
	while(fa[root]) { ++root; }
	dfs(root);
	cout << max(dp[root][1],dp[root][0]);
	return 0;
} 
