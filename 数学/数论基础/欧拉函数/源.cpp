#include<bits/stdc++.h>
using namespace std;
int phi(int n)
{
	int res = n;
	for (int i = 2; i * i <= n; i++)
	{
		if (n % i == 0) res = res / i * (i - 1);
		while (n % i == 0) n /= i;
	}
	if(n > 1) res = res / n * (n - 1);
	return res;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n,q;
	//注意: n >= 2
	cin >> n;
	q = phi(n);
	if (q == n - 1) cout << n << "为质数\n";
	else cout << n << "为合数\n";
	cout << n << "的欧拉函数的值为: " << q;
	
	return 0;
}
