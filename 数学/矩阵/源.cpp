#include<bits/stdc++.h>
using namespace std;
void read_mat(vector<vector<long long>>& mat,int n,int m)
{
	mat.assign(n + 1, vector<long long>(m + 1, 0));
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cin >> mat[i][j];
		}
	}
}
void write_mat(const vector<vector<long long>>& mat)
{
	if (mat.empty())
	{
		cout << "NO mull" << ' ';
		return;
	}
	for (int i = 1; i < mat.size(); i++)
	{
		for (int j = 1; j < mat[1].size(); j++)
		{
			cout << mat[i][j] << ' ';
		}
		cout << '\n';
	}
}
vector<vector<long long>> mul_mat(const vector<vector<long long>>& A, const vector<vector<long long>>& B)
{
	if (A[1].size() != B.size()) return vector<vector<long long>>();
	int n = A.size(), m = B[1].size();
	vector<vector<long long>> ans(n, vector<long long>(m, 0));
	for (int i = 1; i < n; i++)
	{
		for (int k = 1; k < A[1].size(); k++)
		{
			for (int j = 1; j < m; j++)
			{
				ans[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	vector<vector<long long>> A, B, ans;
	int n, m;
	cin >> n >> m;
	read_mat(A, n, m);
	cin >> n >> m;
	read_mat(B, n, m);
	ans = move(mul_mat(A, B));
	write_mat(ans);
	return 0;
}
