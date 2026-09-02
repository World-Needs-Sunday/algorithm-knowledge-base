#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,T;
	cin >> N >> T;
	vector<long long> A(N + 2);
	for(int i = 1;i <= N;i++) cin >> A[i];
	for(int i = N;i >= 1;i--) A[i] = A[i] - A[i - 1]; 
	for(int i = 1;i <= T;i++)
	{
		int l,r;
		long long w;
		cin >> l >> r >> w;
		A[l] += w;
		A[r + 1] -= w;
	}
	for(int i = 1;i <= N;i++)
	{
		A[i] += A[i - 1];
		cout << A[i] << ' ';
	}
	return 0;
 } 
