#include<bits/stdc++.h>
#include<random>
using namespace std;
int n,m;
vector<vector<int>> A;
int main()
{
	//题目生成 
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis1(1,10);
	uniform_int_distribution<int> dis2(1,10000);
	n = dis1(gen);
	A.resize(n + 2,vector<int>(n + 2,0));
	for(int i = 1;i <= n;i++)
	{
		for(int j = 1;j <= i;j++)
		{
			A[i][j] = dis2(gen);
			cout << A[i][j] << '\t';
		}
		cout << endl;
	}
	
	//模板
	vector<vector<int>> dp(n + 2,vector<int>(n + 2 , 0));
	for(int i = 1;i <= n;i++)
	{
		for(int j = 1;j <= i;j++)
		{
			dp[i][j] = max(dp[i - 1][j - 1] , dp[i - 1][j]) + A[i][j];
		}
	} 
	int ans = 0;
	for(int i = 1;i <= n;i++) ans = max(ans,dp[n][i]);
	cout << ans;
	return 0;
}
