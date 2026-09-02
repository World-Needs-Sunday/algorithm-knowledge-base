#include<bits/stdc++.h>
using namespace std;
//字符串编辑距离就是从一个字符串修改到另一个字符串时，编辑单个字符（比如修改、插入、删除）所需要的最少次数
int main()
{
	string a,b;
	cin >> a >> b;
	vector<vector<int>> dp(a.size() + 1,vector<int>(b.size() + 1 , 0));
	//边界初始化 
	for(int i = 1;i <= a.size();i++) dp[i][0] = i;//将a[0] ~ a[i]转化为空所要的次数 
	for(int j = 1;j <= b.size();j++) dp[0][j] = j;//将b[0] ~ b[j]转化为空所要的次数 
	
	
	for(int i = 1;i <= a.size();i++)
	{
		for(int j = 1;j <= b.size();j++)
		{
			if(a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1];
			else
			{
				dp[i][j] = min({
					dp[i - 1][j - 1],/*修改操作，将A[i]与B[j]的位置改成一样*/
					dp[i][j - 1],/*插入操作，将A[i]后面加一个*/
					dp[i- 1][j],/*删除操作,将A[i]删除*/ 
				}) + 1;
			}
		}
	}
	cout << dp[a.size()][b.size()];
	return 0;
}
