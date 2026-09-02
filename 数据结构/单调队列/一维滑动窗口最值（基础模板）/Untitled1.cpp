#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,k;
	cin >> n >> k;
	vector<int> A(n + 1,0);
	for(int i = 1;i <= n;i++) cin >> A[i];
	
	vector<int> mymax;
	vector<int> mymin;
	deque<pair<int,int>> que_max;
	deque<pair<int,int>> que_min;
	for(int i = 1;i <= n;i++)
	{
		if(!que_max.empty())
		{
			int idx = que_max.front().second;
			if(i - idx >= k) que_max.pop_front();
		}
		if(!que_min.empty())
		{
			int idx = que_min.front().second;
			if(i - idx >= k) que_min.pop_front();
		}
		
		while(!que_max.empty() && que_max.back().first <= A[i]) que_max.pop_back();
		que_max.emplace_back(A[i] , i);
		while(!que_min.empty() && que_min.back().first >= A[i]) que_min.pop_back();
		que_min.emplace_back(A[i] , i);
		
		if(i >= k)
		{
			mymax.emplace_back(que_max.front().first);
			mymin.emplace_back(que_min.front().first);
		}
		
	}
	
	for(int i : mymin) cout << i << " ";
	cout << endl;
	for(int i : mymax) cout << i << " ";
	return 0;
}
