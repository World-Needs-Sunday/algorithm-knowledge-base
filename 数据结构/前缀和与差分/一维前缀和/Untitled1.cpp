#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,T;
	cin >> N >> T;
	vector<long long> A(N + 1);
	for(int i = 1;i <= N;i++) cin >> A[i];
	for(int i = 1;i <= N;i++) A[i] += A[i - 1];
	for(int i = 1;i <= T;i++)
	{
		int l,r;
		cin >> l >> r;
		cout << A[r] - A[l - 1] << '\n';
	}
	return 0;
 } 
