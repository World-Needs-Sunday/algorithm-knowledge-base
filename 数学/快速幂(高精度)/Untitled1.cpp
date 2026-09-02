#include<bits/stdc++.h>
using namespace std;
const int n = 510;
char A[n];
char B[n];
int ans[n];
void mul(char tA[],char tB[])
{
	memset(ans,0,sizeof(ans));
	int na = 500,nb = 500;
	while(!tA[na - 1]) --na;
	while(!tB[nb - 1]) --nb;
	for(int i = 0;i < na;i++)
	{
		for(int j = 0;j < nb;j++)
		{
			if(i + j < 500) ans[i + j] += tA[i] * tB[j];
		}
	}
	for(int i = 0;i < 500;i++)
	{
		ans[i + 1] += ans[i] / 10;
		ans[i] %= 10;
	}
}
void fastpow(int q)
{
	if(q == 0)
	{
		A[0] = 1;
		return;
	}
	A[0] = 1;
	B[0] = 2;
	while(q)
	{
		if(q & 1)
		{
			mul(A,B);
			for(int i = 0;i < 500;i++) A[i] = ans[i];
		}
		mul(B,B);
		for(int i = 0;i < 500;i++) B[i] = ans[i];
		q >>= 1;
	}
}
int main()
{
	int q;
	scanf("%d",&q);
	int num = q * log10(2) + 1;
	printf("%d\n",num);
	fastpow(q);
	A[0] -= 1;
	reverse(A,A + 500);
	for(int i = 0;i < 500;i++)
	{
		if(i && i % 50 == 0) putchar('\n');
		putchar(A[i] + 48);
	}
	return 0; 
} 
