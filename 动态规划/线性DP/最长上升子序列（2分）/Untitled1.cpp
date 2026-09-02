#include<bits/stdc++.h>
using namespace std;
int n;
vector<int> A;
vector<int> B;
int find(int x)
{
	int l = 0,r = B.size() - 1;
	while(l < r)
	{
		int mid = (l + r) / 2;
		if(x >= B[mid]) r = mid;
		else l = mid + 1;
	}
	return l;
}
int main()
{
	cin >> n;
	A.resize(n + 1,0);
	B.emplace_back(0);
	for(int i = 1;i <= n;i++) cin >> A[i];
	for(int i = 1;i <= n;i++)
	{
		if(A[i] > B.back()) B.emplace_back(A[i]);
		else B[find(A[i])] = A[i];
	}
	cout << B.size() - 1;
	return 0;	
} 
