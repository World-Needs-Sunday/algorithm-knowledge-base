#include<bits/stdc++.h>
using namespace std;
/*洛谷1892*/
vector<int> A;
void init(int n)
{
	A.resize(n * 2 + 1);
	for(int i = 1;i <= n * 2;i++) A[i] = i;
}
int find(int i)
{
	return A[i] == i ? i : find(A[i]);
}
void unity(int a,int b)
{
	A[find(b)] = find(a);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,m;
	cin >> n >> m;
	init(n);
	for(int i = 0;i < m;i++)
	{
		char op;
		int p,q;
		cin >> op >> p >> q;
		if(op == 'F') unity(p,q);
		else
		{
			unity(p,q + n);
			unity(q,p + n);
		}
	}
	int ans = 0;
	for(int i = 1;i <= n;i++) if(i == A[i]) ++ans;
	cout << ans;
	return 0;	
} 
