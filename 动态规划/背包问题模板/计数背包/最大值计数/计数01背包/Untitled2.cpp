#include<bits/stdc++.h>
using namespace std;
const long long k = 1e9 + 7;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,V;
	cin >> N >> V;
	vector<long long> cnt(V + 1,1);
	vector<long long> nmax(V + 1,0);
	for(int i = 1;i <= N;i++)
	{
		int v,m;
		cin >> v >> m;
		for(int j = V;j >= 1;j--)
		{
			if(j >= v) 
			{
				if(nmax[j - v] + m > nmax[j])
				{
					nmax[j] = nmax[j - v] + m;
					cnt[j] = cnt[j - v];
				}
				else if(nmax[j - v] + m == nmax[j]) cnt[j] = (cnt[j] + cnt[j - v]) % k;	
			}
		}
	}
	cout << cnt[V];
	return 0; 
}
