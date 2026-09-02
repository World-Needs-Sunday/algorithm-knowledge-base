#include<bits/stdc++.h>
using namespace std;
/*洛谷P3367 时间1.90s*/ 
vector<int> A;
vector<int> rt; 
int n;
void init(int n)
{
	A.resize(n + 1);
	rt.resize(n + 1,1);
	for(int i = 1;i <= n;i++) A[i] = i;
}
int find(int i)
{
	if(A[i] != i)
	{
		A[i] = find(A[i]);
	}
	return A[i];
}
void unity(int a,int b)
{
	int ra = find(a);
	int rb = find(b);
	if(ra == rb) return;
	if(rt[ra] != rt[rb])
	{
		if(rt[ra] > rt[rb]) A[rb] = ra;
		else A[ra] = rb;
	}
	else
	{
		A[rb] = ra;
		++rt[ra];
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr); 
	int N,M;
	cin >> N >> M;
	init(N);
	for(int i = 1;i <= M;i++)
	{
		int op,a,b;
		cin >> op >> a >> b;
		if(op == 1) unity(a,b);
		else 
		{
			if(find(a) == find(b)) cout << "Y" << '\n';
			else cout << "N" << '\n'; 
		}
	}
	return 0;
} 
