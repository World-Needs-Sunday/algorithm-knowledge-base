#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,M,T;
	cin >> N >> M >> T;
	vector<vector<long long>> A(N + 1,vector<long long>(M + 1));
	for(int i = 1;i <= N;i++)
	{
		for(int j = 1;j <= M;j++)
		{
			cin >> A[i][j];
		}
	}
	for(int i = 1;i <= N;i++)
	{
		for(int j = 1;j <= M;j++)
		{
			A[i][j] = A[i][j] + A[i - 1][j] + A[i][j - 1] - A[i - 1][j - 1];
		}
	}
	
	for(int i = 1;i <= T;i++)
	{
		int x1,y1,x2,y2;
		//保证(x1,y1)是左上角，(x2,y2)是右下角 
		cin >> x1 >> y1 >> x2 >> y2;
		cout << A[x2][y2] - A[x1 - 1][y2] - A[x2][y1 - 1] + A[x1 - 1][y1 - 1] << '\n';
	}
	return 0;
 } 
