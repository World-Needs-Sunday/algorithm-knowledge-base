#include<bits/stdc++.h>
using namespace std;
/*给定若干区间，合并所有存在重叠或相邻的区间，输出互不相交的最终区间集合。*/
const int INF = INT_MAX;
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
		sort(A.begin() + 1,A.end());
		A.emplace_back(INF,INF); 
		
		int L = INF,R = -INF;
		int cnt = 0;
		vector<pair<int,int>> ans(n + 1); 
		for(int i = 1;i <= n + 1;i++)
		{
			if(A[i].first <= R) R = max(A[i].second,R); 
			else
			{
				ans[cnt].first = L;
				ans[cnt].second = R;
				++cnt;
				L = A[i].first;
				R = A[i].second;
			}
		}
		
		cout << "合并后会 " << cnt - 1 << " 区间" <<'\n';
		for(int i = 1;i < cnt;i++)
		{
			cout <<"长度为:"<<ans[i].second - ans[i].first + 1 << "  L = " << ans[i].first << "  R = " << ans[i].second << '\n';
		}
	}
	return 0;	
} 
