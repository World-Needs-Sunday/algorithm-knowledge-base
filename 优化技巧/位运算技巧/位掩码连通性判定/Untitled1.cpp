#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
vector<unsigned int> edges;

//判断是否联通
bool is_cond(unsigned int mask)
{
	if (!(mask & (mask - 1))) return true; //没有点和一个点看为联通
	unsigned int seed = mask & (~mask + 1); //等价与mask & (-mask)
	unsigned int comp = seed; //已经确定相连的点

	unsigned int bfs_que = seed; //模拟bfs队列，即下一轮要处理的点

	while (bfs_que)
	{
		//处理bfs_que可以连接的边
		unsigned nxt = 0;
		for (unsigned int f = bfs_que; f; f &= f - 1) nxt |= edges[__builtin_ctz(f)];

		nxt &= mask; // 将不属于mask的点去除
		nxt &= ~comp; // 将已经在连通图的去掉
		comp |= nxt; // 将本轮可以加的点加入comp
		bfs_que = nxt; //下一轮要处理的点
	}

	return comp == mask;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//点的下标为0~base - 1    正常题目是1 ~ base 记得要映射成0~base - 1
	int n, m, k;
	cin >> n >> m >> k;
	edges.assign(n, 0);
	for (int i = 1; i <= m; i++)
	{
		int a, b;
		cin >> a >> b;
		a -= 1, b -= 1;
		edges[a] |= (unsigned int)1 << b;
		edges[b] |= (unsigned int)1 << a;
	}
	for (int i = 1; i <= k; i++)
	{
		unsigned int cost;
		cin >> cost;
		if (is_cond(cost)) cout << "是联通块(或为空)\n";
		else cout << "不是连通块\n";
	}
	return 0;	
}
