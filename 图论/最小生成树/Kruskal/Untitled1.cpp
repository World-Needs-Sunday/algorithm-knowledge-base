#include<bits/stdc++.h>
using namespace std;
struct edge
{
	int u,v;
	long long w;
	edge(){}
};
struct cmp
{
	bool operator() (const edge& a,const edge& b)
	{
		return a.w < b.w;
	}
};
int N,M;
vector<int> A;
void init()
{
	A.resize(N + 1,0);
	for(int i = 1;i <= N;i++) A[i] = i;
}
int find(int i)
{
	if(A[i] != i) A[i] = find(A[i]);
	return A[i];
}
void unity(int a,int b)
{
	A[find(b)] = find(a);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	cin >> N >> M;
	init();
	vector<edge> edges(M + 1);
	for(int i = 1;i <= M;i++) cin >> edges[i].u >> edges[i].v >> edges[i].w;
	sort(edges.begin() + 1,edges.end(),cmp());
	int cnt = 0;
	long long ans = 0;
	for(int i = 1;i <= M;i++)
	{
		int u = edges[i].u;
		int v = edges[i].v;

		if(find(u) != find(v))
		{
			unity(u,v);
			ans += edges[i].w;
			++cnt;
		}

		if(cnt == N - 1) break;
	}
	if(cnt == N - 1) cout << ans;
	else cout << "orz";
	return 0;
}
