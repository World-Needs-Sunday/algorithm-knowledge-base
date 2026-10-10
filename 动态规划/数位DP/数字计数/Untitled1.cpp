#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

vector<long long> fn(long long num)
{
	vector<long long> ans(10,0);
	for(long long i = 1;i <= num;i *= 10)
	{
		long long num_h = num / i / 10;
		long long num_d = num % (i * 10);
		for(int d = 0;d <= 9;d++)
		{
			if(d == 0)
			{
				long long p = num_h - 1;
				if(p < 0) continue;
				long long q = num_d - i * d;
				ans[d] += p * i;
				if(q >= i) ans[d] += i;
				else if(q >= 0) ans[d] += q + 1;
			}
			else
			{
				long long p = num_h;
				long long q = num_d - i * d;
				ans[d] += p * i;
				if(q >= i) ans[d] += i;
				else if(q >= 0) ans[d] += q + 1;
			}
		}
	}	
	return ans;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	long long l,r;
	cin >> l >> r;
	vector<long long> t1 = fn(r);
	vector<long long> t2 = fn(l - 1);
	for(int i = 0;i <= 9;i++) cout << t1[i] - t2[i] << ' ';
	return 0;
}
