#include<bits/stdc++.h>
using namespace std;
long long fastpow(long long a,long long b,long long q)
{
	if(b == 0) return 1;
	long long ans = 1;
	long long gap = a;
	while(b)
	{
		if(b & 1) ans = (ans * gap) % q;
		gap = (gap * gap) % q;
		b >>= 1;
	}
	return ans;
}
int main() 
{
	long long a,b,q;
	cin >> a >> b >> q;
	cout << a << "^" << b << " mod " << q << "=" << fastpow(a,b,q);
	return 0;
}
