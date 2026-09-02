#include<bits/stdc++.h>
using namespace std;
/*给定若干区间，选出最多数量互不重叠的区间 (起点可以相交)*/
struct cmp
{
	bool operator() (pair<int,int>& a,pair<int,int>& b)
	{
		if(a.second != b.second) return a.second < b.second;
		return a.first < b.first;
	}
};
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int T;
	cin >> T;
	while(T--)
	{
		int n;
		cin >> n;
		vector<pair<int,int>> A(n + 1);
		for(int i = 1;i <= n;i++) cin >> A[i].first >> A[i].second;
		sort(A.begin() + 1,A.end(),cmp()); 
		int from = -0x3f3f3f3f,cnt = 0;
		for(int i = 1;i <= n;i++)
		{
			if(A[i].first >= from)
			{
				from = A[i].second;
				++cnt;
			}
		}
		cout << cnt << '\n';
	}
	return 0;	
} 
