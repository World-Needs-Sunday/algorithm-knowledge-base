#include<bits/stdc++.h>
using namespace std;
inline int lowbit(int x)
{
	return x & -x;
}

struct treearr
{
	vector<long long> arr;
	int n;
	treearr() {}
	treearr(int k)
	{
		n = k;
		arr.assign(n + 1,0);
	}
	
	void add(int idx,long long k)
	{
		while(idx <= n)
		{
			arr[idx] += k;
			idx += lowbit(idx);
		}
	}
	
	long long get(int idx)
	{
		long long ans = 0;
		while(idx)
		{
			ans += arr[idx];
			idx -= lowbit(idx);
		}
		return ans;
	}
};
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,M;
	cin >> N >> M;
	treearr S1(N),S2(N);
	for(int i = 1;i <= N;i++)
	{
		long long x;
		cin >> x;
		S1.add(i,x);
		S2.add(i,x * i);
	}
	
	for(int i = 1;i <= M;i++)
	{
		string str;
		cin >> str;
		if(str == "Modify")
		{
			int idx;
			long long x;
			cin >> idx >> x;
			long long k = x - S1.get(idx) + S1.get(idx - 1);
			S1.add(idx,k);
			S2.add(idx,k * idx);
		}
		else if(str == "Query")
		{
			int idx;
			cin >> idx;
			cout << (long long)(idx + 1) * S1.get(idx) - S2.get(idx) << '\n';
		}
	}
	return 0;
}
