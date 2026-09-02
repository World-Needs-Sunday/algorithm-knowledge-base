#include<bits/stdc++.h>
using namespace std;
inline void read(vector<int>& arr)
{
	arr.clear();
	arr.emplace_back(0);
	char ch = getchar();
	while (ch < '0' || ch >'9') ch = getchar();
	while (ch >= '0' && ch <= '9')
	{
		arr.emplace_back(ch - 48);
		ch = getchar();
	}
}
inline void write(int num)
{
	int id = 0;
	static char ans[12];
	if (num < 0) putchar('-'), num = -num;
	while (num)
	{
		ans[id++] = num % 10 + 48;
		num /= 10;
	}
	while (id--) putchar(ans[id]);
	putchar('\n');
}
int main()
{
	int t;
	scanf("%d", &t);
	while (t--)
	{
		vector<int> A;
		vector<int> B;
		read(A); read(B);
		for (int i = 1; i < A.size(); i++) A[i] = (A[i] + A[i - 1]) % 10;
		for (int i = 1; i < B.size(); i++) B[i] = (B[i] + B[i - 1]) % 10;
		vector<vector<int>> dp(A.size(), vector<int>(B.size(), 0));
		for (int i = 1; i < A.size(); i++)
		{
			for (int j = 1; j < B.size(); j++)
			{
				if (A[i] == B[j]) dp[i][j] = dp[i - 1][j - 1] + 1;
				else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
		if (A.back() == B.back()) write(dp[A.size() - 1][B.size() - 1]);
		else write(-1);
	}
	return 0;
}
