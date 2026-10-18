#include<bits/stdc++.h>
using namespace std;
/*
当字符串 S 中选定一个位置 i 满足 S [i~n] + S [1~i-1] = T，
则 T 是 S 的循环同构串。
设 S=“bcad”，其循环同构串有 “bcad”、“cadb”、“adbc”、“dbca”，
当 i=3 时，得到字典序最小的循环同构串是 “adbc”。

最小表示法就是找出字符串 S 的循环同构串中字典序最小的那一个。
*/

int fn(string str)
{
	int n = str.size();
	if (n == 0) return 0;

	str += str;
	int i = 0;
	int j = 1;

	while (i < n && j < n)
	{
		int k = 0;
		while (k < n && str[i + k] == str[j + k]) k++;

		if (k == n) break; 

		if (str[i + k] < str[j + k])
		{
			j = j + k + 1;
			if (j == i) j++;
		}
		else
		{
			i = i + k + 1;
			if (i == j) i++;
		}
	}

	return min(i, j);
}
int main()
{
	string str;
	cin >> str;
	int idx = fn(str);
	int n = str.size();
	for (int i = 0; i < n; i++) cout << str[(i + idx) % n];
	return 0;
}



/* 暴力的做法
bool cmp(const string& str,int i,int j,int n)
{
	for(int idx = 0;idx < n;idx++)
	{
		if(str[i + idx] == str[j + idx]) continue;
		else return str[i + idx] > str[j + idx];
	}
	return false;
}
int main()
{
	string str;
	cin >> str;
	int n = str.size();
	str = str + str;
	int j = 0;
	for(int i = 1;i < n;i++)
	{
		if(cmp(str,i,j,n)) j = i;
	}
	for(int i = 0 ;i < n;i++) cout << str[j + i];
	return 0;
}
*/
