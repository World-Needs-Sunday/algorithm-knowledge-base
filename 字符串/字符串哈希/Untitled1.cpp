#include<bits/stdc++.h>
using namespace std;
/*
	字符串哈希 把不同的字符串映射成不同的整数。
	把字符串映射成一个 p 进制数字。
	对于一个长度为 n 的字符串 s，
*/
const int q = 131;
const int nmax = 10010;
vector<unsigned long long>P;
vector<unsigned long long> sum;
unsigned long long getstr(int l , int r)
{
	return sum[r] - sum[l - 1] * P[r - l + 1];
}
int main()
{
	string str;
	cin >> str;
	sum.resize(str.size() + 1 , 0);
	P.resize(str.size() + 1 , 1);
	for(int i = 0;i < str.size();i++) sum[i + 1] = sum[i] * q + (unsigned long long)str[i]; 
	for(int i = 1;i <= str.size();i++) P[i] = P[i - 1] * q;
	int k;
	cin >> k;
	for(int i = 0;i < k;i++)
	{
		int l , r;
		cin >> l >> r;
		cout << getstr(l,r) << endl;
	}
	return 0;	
}
