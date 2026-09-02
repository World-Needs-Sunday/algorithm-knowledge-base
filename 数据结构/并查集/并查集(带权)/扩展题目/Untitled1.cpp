#include<bits/stdc++.h>
using namespace std;
/*洛谷P2024*/
vector<int> A;
vector<long long> d;
void init(int n)
{
	A.resize(n + 1);
	for(int i = 1;i <= n;i++) A[i] = i;
	d.resize(n + 1,0);
}
int find(int idx)
{
	if(idx != A[idx])
	{
		int old = A[idx]; 
		A[idx] = find(A[idx]);
		d[idx] = (d[old] + d[idx]) % 3;
	}
	return A[idx];
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr); 
	int N,K;
	int cnt = 0;
	cin >> N >> K;
	init(N); 
	for(int i = 1;i <= K;i++)
	{
		int op,X,Y;
		cin >> op >> X >> Y;
		if(X > N || Y > N)
		{
			++cnt;
			continue;
		}
		int rx = find(X);int ry = find(Y); 
		if(op == 1)
		{
			if(rx == ry)
			{
				if(d[X] % 3 != d[Y] % 3) ++cnt;
			}
			else
			{
				A[ry] = rx;
				d[ry] = (d[X] - d[Y] + 3) % 3;
			}
		}
		else
		{
			if(rx == ry)
			{
				if((d[Y] - d[X] + 3) % 3 != 1) ++cnt;
			}
			else 
			{
				A[ry] = rx;
				d[ry] = (d[X] - d[Y] + 4) % 3;
			}
		}
	}
	cout << cnt;
	return 0;	
} 
