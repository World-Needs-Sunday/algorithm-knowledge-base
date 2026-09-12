#include<bits/stdc++.h>
using namespace std;
vector<vector<char>> T;
int f_x[4] = { 1,-1,0,0 };
int f_y[4] = { 0,0,1,-1 };
int bfs(pair<int,int> begin_pos,pair<int,int> end_pos,int n,int m)
{
	vector<pair<int, int>> que(n * m + 10);
	int l = 0, r = 0;
	vector<vector<short>> vis(n + 1, vector<short>(m + 1,0));
	vector<vector<int>> A(n + 1, vector<int>(m + 1, INT_MAX));
	que[r++] = begin_pos;
	vis[begin_pos.first][begin_pos.second] = 1;
	A[begin_pos.first][begin_pos.second] = 0;

	que[r++] = end_pos;
	vis[end_pos.first][end_pos.second] = 2;
	A[end_pos.first][end_pos.second] = 0;

	while (l < r)
	{
		pair<int, int> tmp = que[l++];
		for (int i = 0; i < 4; i++)
		{
			int tx = tmp.first + f_x[i];
			int ty = tmp.second + f_y[i];
			if (tx < 1 || tx > n || ty < 1 || ty > m || vis[tmp.first][tmp.second] == vis[tx][ty]|| T[tx][ty] == '#') continue;
			if (vis[tmp.first][tmp.second] + vis[tx][ty] == 3)
			{
				return A[tmp.first][tmp.second] + A[tx][ty] + 1;
			}
			vis[tx][ty] = vis[tmp.first][tmp.second];
			A[tx][ty] = A[tmp.first][tmp.second] + 1;
			que[r++] = pair<int, int>(tx, ty);
		}
	}
	return -1;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	T.assign(n + 1, vector<char>(m + 1));
	//#是不能走   .是可以走
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cin >>T[i][j];
		}
	}
	pair<int, int> a, b;
	cin >> a.first >> a.second;
	cin >> b.first >> b.second;
	if (a == b) cout << 0;
	else cout << bfs(a, b, n, m);
	return 0;
}
