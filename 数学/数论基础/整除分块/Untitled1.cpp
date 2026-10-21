#include<bits/stdc++.h>
using namespace std;
//求取sum(i = 1 ~ n) floor(n / i);   (连续型分块)
long long fn1(long long n)
{
	/*
	可以发现在求取的过程中有很多重复的数字。并且具有单调性
	假设一个相同值的区间是[L,R];则floor(n / L) == floor(n / R)
	设 k = floor(n / L)。则:  k <= n / R < k + 1 
	所以  k * R <= n < (k + 1) * R
	n / (k + 1) <  R <= n / k ,  R = floor(n / k)
	第一个起点L = 1,下一个不同值的区间为R + 1
	*/
	long long ans = 0;
	for(long long l = 1,r;l <= n;l = r + 1)
	{
		long long k = n / l;
		r = n / k;
		ans += k * (r - l + 1);
	}
	return ans;
}

//求取sum(i = 1 ~ n) n % i;    (余数型分块)
long long fn2(long long n)
{
	/*
	sum(i = 1 ~ n) n % i 可以看成  sum(i = 1 ~ n) n - floor(n / i) * i;
	这一部分：sum(i = l ~ r) floor(n / i) * i 的求和可以写成 len * (l + r) / 2 * k;
	后面过程与前面差不多 
	*/
	long long ans = 0;
	for(long long l = 1,r;l <= n;l = r + 1)
	{
		long long k = n / l;
		r = n / k;
		long long len = r - l + 1;
		ans += len * n - len * (l + r) / 2 * k;  
	} 
	return ans;
}

//倍增型分块 不具体题目了，就把每个长度的个数打印一下吧。 
void fn3(long long n,long long k)
{
	/*
	不知道怎么写，给ai了 
	把 1..n 中的数按“根 a 不被 k 整除”分链：
		根 a 的链为 a, a*k, a*k^2, ...
		链长 L(a) = 满足 a*k^t <= n 的最大 t + 1

	设 U_t = floor(n / k^t)，则:  U_0 = n,  U_{t+1} = floor(U_t / k)
	（因为 floor(floor(n/A)/B) = floor(n/(A*B))，取 A=k^t, B=k 即得）

	链长恰好为 t+1 的根 a 满足:
		a*k^t <= n < a*k^{t+1}
	两边除以 k^t:
		a <= n/k^t 且 a > n/k^{t+1}
	对整数 a:
		U_{t+1} < a <= U_t
	所以候选根是连续区间 (U_{t+1}, U_t]

	但根必须 k ? a，要从该区间里排除 k 的倍数:
	区间长度 = U_t - U_{t+1}
	其中 k 的倍数个数 = floor(U_t/k) - floor(U_{t+1}/k)
					= U_{t+1} - U_{t+2}
	（因为 floor(U_t/k) = U_{t+1}，floor(U_{t+1}/k) = U_{t+2}）

	所以链长为 t+1 的根个数:
		cnt = (U_t - U_{t+1}) - (U_{t+1} - U_{t+2})
			= U_t - 2*U_{t+1} + U_{t+2}

	代码变量替换:  U = U_t,  D = U_{t+1},  D/k = U_{t+2}
		cnt = U - 2*D + D/k
	下一层:  U = D,  len++（链长递增）
	层数只有 O(log_k n)，非常快
	注意: k = 1 时 U 不降，会死循环，必须特判
	*/
	
	if (k == 1) 
	{
        cout << "k=1 无意义（所有数都能被1整除，根不存在）\n";
        return;
    }
	
	long long U = n;
	int len = 1;
	while(U > 0)
	{
		long long D = U / k;
		long long cnt = U - 2 * D + D / k;
		
		cout << len << ' ' << cnt << '\n'; 
		
		U = D;
		++len;
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	long long n,k;
	cin >> n >> k;
	cout << fn1(n) << '\n';
	cout << fn2(n) << '\n';
	fn3(n,k); 
	return 0;
}
