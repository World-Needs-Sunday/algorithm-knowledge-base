#include<bits/stdc++.h>
using namespace std;
int n_x[3] = { -1 , -1 , 0};
int n_y[3] = { -1 , 0 , -1};
int main()
{
	string a,b;
	cin >> a >> b;
	vector<vector<int>> dp(a.size() + 1,vector<int>(b.size() + 1,0));
	vector<vector<int>> A(a.size() + 1,vector<int>(b.size() + 1 , 0)); 
	for(int i = 1;i <= a.size();i++)
	{
		for(int j = 1;j <= b.size();j++)
		{
			if(a[i - 1] == b[j - 1]) 
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
				A[i][j] = 0;
			}
			else if(dp[i - 1][j] > dp[i][j - 1])
			{
				dp[i][j] = dp[i - 1][j];
				A[i][j] = 1;
			}
			else
			{
				dp[i][j] = dp[i][j - 1];
				A[i][j] = 2;
			}
		}
	}
	vector<char> str(dp[a.size()][b.size()]);
	int idx = dp[a.size()][b.size()] - 1;
	int x = a.size();
	int y = b.size();
	while(x != 0 && y != 0)
	{
		int tmp = A[x][y];
		if(tmp  == 0)
		{
			str[idx--] = a[x - 1];
			x += n_x[tmp];
			y += n_y[tmp];
		}
		else
		{
			x += n_x[tmp];
			y += n_y[tmp];
		}
	}
	cout << "最长子序列的长度为 ： ";
	for(char i : str) cout << i;
	cout << '\n';
	
	cout <<"长度为 :  "<< dp[a.size()][b.size()];
	return 0;
}
