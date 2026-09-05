#include<bits/stdc++.h>
using namespace std;
/*洛谷P1050*/
struct num
{
	int v[105];
	num() { memset(v, 0, sizeof(v));}
};
void read(num& tmp)
{
	char ch = getchar();
	int idx = 0;
	while (ch <= '9' && ch >= '0')
	{
		tmp.v[idx++] = ch - 48;
		ch = getchar();
	}
	reverse(tmp.v, tmp.v + idx);
}
struct num mul(num& a, num& b, int k)
{
	num ans;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			if(i + j < k) ans.v[i + j] += a.v[i] * b.v[j];
		}
	}
	for (int i = 0; i < k; i++)
	{
		ans.v[i + 1] += ans.v[i] / 10;
		ans.v[i] %= 10;
	}
	return ans;
}
int ans[200];
int n = 1;
void add(int j)
{
	int tmp[200];
	memset(tmp, 0, sizeof(tmp));
	for (int i = 0; i < n; i++) tmp[i] = ans[i] * j;
	int i = 0;
	while (i < n || tmp[i] != 0)
	{
		tmp[i + 1] += tmp[i] / 10;
		tmp[i] %= 10;
		++i;
	}
	n = i;
	for (int i = 0; i < n; i++) ans[i] = tmp[i];
}
int main()
{
	//开始计时 
	clock_t start = clock();
	
	int k;
	num A;
	read(A);
	scanf("%d", &k);

	num tg = A;
	ans[0] = 1;
	for (int i = 0; i < k; i++)
	{
		bool fg = false;
		num tmp = mul(A,tg,k);
		if (tmp.v[i] == A.v[i]) continue;
		else
		{
			num newtg = tg;
			for (int j = 2; j <= 10; j++)
			{
				newtg = mul(newtg, tg, k);
				if (mul(A, newtg, k).v[i] == A.v[i])
				{
					add(j);
					fg = true;
					tg = newtg;
					break;
				}
			}
			if (!fg)
			{
				printf("-1");
				return 0;
			}
		}
	}
	n -= 1;
	while (n >= 0)
	{
		putchar(ans[n] + 48);
		--n;
	}
	
	//结束计时 
	clock_t end = clock();
	double avg = (double)(end-start)/CLOCKS_PER_SEC*1000 / 1000;
	printf("单次平均耗时：%.3f 毫秒\n",avg);
	return 0;
}
