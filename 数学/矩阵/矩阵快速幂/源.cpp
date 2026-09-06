#include<bits/stdc++.h>
using namespace std;
const long long mod = 1e9 + 7;
void read_mat(vector<vector<long long>>& mat, int n, int m)
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
vector<vector<long long>> mul_mat(const vector<vector<long long>>& A, const vector<vector<long long>>& B, const long long mod)
{
	if (A[1].size() != B.size()) return vector<vector<long long>>();
	int n = A.size(), m = B[1].size();
	vector<vector<long long>> ans(n, vector<long long>(m, 0));
	for (int i = 1; i < n; i++)
	{
		for (int k = 1; k < A[1].size(); k++)
		{
			if (!A[i][k]) continue;
			for (int j = 1; j < m; j++)
			{
				ans[i][j] = (ans[i][j] + A[i][k] * B[k][j]) % mod;
			}
		}
	}
	return ans;
}

vector<vector<long long>> fastpow_mat(const vector<vector<long long>>& A, long long k, const long long mod)
{
	if (A.size() != A[1].size()) return vector<vector<long long>>();
	vector<vector<long long>> ans(A.size(), vector<long long>(A[1].size(), 0));
	for (int i = 1; i < A.size(); i++) ans[i][i] = 1;
	vector<vector<long long>> tmp(A);
	if (k == 0) return ans;
	while (k)
	{
		if (k & 1) ans = move(mul_mat(tmp, ans, mod));
		tmp = move(mul_mat(tmp, tmp, mod));
		k >>= 1;
	}
	return ans;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	vector<vector<long long>> A, ans;
	int n;
	long long k;
	cin >> n >> k;
	read_mat(A, n, n);
	ans = move(fastpow_mat(A, k, mod));
	write_mat(ans);
	return 0;
}
