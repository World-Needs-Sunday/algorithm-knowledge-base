#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> A;
bool cmp(int a,int b,bool k)
{
	return k ? a <= b : a >= b;
}
void fn(int M,int N,bool k,int big,vector<vector<int>>& tmp)
{
	tmp.resize(M + 1, vector<int>(N + 1));
	vector<pair<int, int>> que_x(N + 2);
	for (int i = 1; i <= M; i++)
	{
		int x_l = 0, x_r = 0;
		for (int j = 1; j <= N; j++)
		{
			int a = A[i][j];
			if (x_l != x_r && j - que_x[x_l].first >= big) ++x_l;
			while (x_l != x_r && cmp(a, que_x[x_r - 1].second, k)) --x_r;
			que_x[x_r++] = pair<int, int>(j, a);
			tmp[i][j] = que_x[x_l].second;
		}
	}
	vector<pair<int, int>> que_y(M + 2);
	for (int j = 1; j <= N; j++)
	{
		int y_l = 0, y_r = 0;
		for (int i = 1; i <= M; i++)
		{
			int a = tmp[i][j];
			if (y_l != y_r && i - que_y[y_l].first >= big) ++y_l;
			while (y_l != y_r && cmp(a, que_y[y_r - 1].second, k)) --y_r;
			que_y[y_r++] = pair<int, int>(i, a);
			tmp[i][j] = que_y[y_l].second;
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int M, N, big;
	cin >> M >> N >> big;
	A.resize(M + 1, vector<int>(N + 1));
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			cin >> A[i][j];
		}
	}
	cout << '\n';
	vector<vector<int>> n_min, n_max;
	fn(M, N, true, big, n_min);
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			cout << n_min[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
	fn(M, N, false, big, n_max);
	for (int i = 1; i <= M; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			cout << n_max[i][j] << ' ';
		}
		cout << '\n';
	}
	return 0;
	
}
