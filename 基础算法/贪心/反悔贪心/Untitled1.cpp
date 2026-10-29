#include<bits/stdc++.h>
using namespace std;
priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> A,B,T;
vector<int> P,C;
vector<bool> vis;
void init(int n)
{
	P.assign(n + 1,0);
	C.assign(n + 1,0);
	vis.assign(n + 1,false);
}
int main()
{
	ios::sync_with_stdio(false);
	int N,K;
	long long M;
	cin >> N >> K >> M;
	init(N);
	for(int i = 1;i <= N;i++)
	{
		cin >> P[i] >> C[i];
		A.emplace(P[i],i);
		B.emplace(C[i],i);
	}
	
	long long w = 0;
	int cnt = 0;
	while(!A.empty() && !B.empty())
	{
		int id_a = A.top().second;
		if(vis[id_a])
		{
			A.pop();
			continue;
		}
		
		int id_b = B.top().second;
		if(vis[id_b])
		{
			B.pop();
			continue;
		}
		
		if((int)T.size() < K)
		{
			B.pop();
			vis[id_b] = true;
			w += C[id_b];
			T.emplace(P[id_b] - C[id_b] , id_b);
		}
		else
		{
			//2种选择   1，选择原价 w += P[id_a]    2,选择优惠价 w += C[id_b] + T.top().first;
			if(P[id_a] < C[id_b] + T.top().first)
			{
				A.pop();
				vis[id_a] = true;
				w += P[id_a];
			}
			else
			{
				B.pop();
				vis[id_b] = true;
				w += C[id_b] + T.top().first;
				T.pop();
				T.emplace(P[id_b] - C[id_b],id_b);
			}
		}
		
		if(w > M) break;
		else ++cnt;
	}
	cout << cnt;
	return 0;
}
