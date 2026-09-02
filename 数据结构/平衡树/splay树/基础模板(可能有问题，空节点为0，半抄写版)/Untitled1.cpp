#include<bits/stdc++.h> 
using namespace std;
struct sytree
{
	int fa;
	int son[2];
	int val;
	int sz;
	int cnt;
	sytree(int a)
	{
		son[0] = 0; son[1] = 0;
		val = a;
		cnt = 1;
		fa = 0;
		sz = 1;
	}
};
vector<sytree> tree;
int root = 0;
inline void upsz(int x)//修改大小 
{
	tree[x].sz = tree[tree[x].son[0]].sz + tree[tree[x].son[1]].sz + tree[x].cnt;
}
void rot(int x) //旋转 
{
	int y = tree[x].fa;
	int z = tree[y].fa;
	int k = tree[y].son[1] == x;
	tree[y].son[k] = tree[x].son[k ^ 1];
	tree[tree[y].son[k]].fa = y;
	tree[x].son[k ^ 1] = y;
	tree[y].fa = x;
	tree[z].son[tree[z].son[1] == y] = x;
	tree[x].fa = z;
	upsz(y), upsz(x);
}
void splay(int x, int k)//将x移动到k下面 
{
	while (tree[x].fa != k)
	{
		int y = tree[x].fa;
		int z = tree[y].fa;
		if (z != k)
		{
			(tree[y].son[0] == x) ^ (tree[z].son[0] == y) ? rot(x) : rot(y);//同向旋转y,异向旋转x
		}
		rot(x);
	}
	if (k == 0) root = x;
}
void find(int v)//找到v，并且将x移动到根节点 
{
	int x = root;
	while (tree[x].son[v > tree[x].val] && tree[x].val != v) x = tree[x].son[v > tree[x].val];
	splay(x, 0);
}
int get_pre(int v) //找到v的前驱 
{
	find(v);
	int x = root;
	if (tree[x].val < v) return x;
	x = tree[x].son[0];
	while (tree[x].son[1]) x = tree[x].son[1];
	return x;
}
int get_suc(int v)//找到v的后继 
{
	find(v);
	int x = root;
	if (tree[x].val > v) return x;
	x = tree[x].son[1];
	while (tree[x].son[0]) x = tree[x].son[0];
	return x;
}
void del(int v)//删除v 
{
	int pre = get_pre(v);
	int suc = get_suc(v);
	splay(pre, 0); splay(suc, pre);
	//将pre当根节点，suc作为pre的右孩子，目的是因为v是树中唯一满足pre < v < suc,所以v一定是suc的左孩子
	int tmp = tree[suc].son[0];
	if (tree[tmp].cnt > 1)
	{
		--tree[tmp].cnt;
		splay(tmp, 0);
	}
	else
	{
		tree[suc].son[0] = 0;
		splay(suc, 0);
	}
}
int getrk(int v)//找到v的排名 
{
	find(v);
	return tree[root].val < v ? tree[tree[root].son[0]].sz + tree[root].cnt : tree[tree[root].son[0]].sz;
}
int getval(int k)//找到第k小的数 
{
	int x = root;
	k += 1;
	while (true)
	{
		int t = tree[x].son[0];
		if (tree[t].sz + tree[x].cnt < k)
		{
			k -= tree[t].sz + tree[x].cnt;
			x = tree[x].son[1];
		}
		else
		{
			if (tree[t].sz >= k) x = tree[x].son[0];
			else break;
		}
	}
	splay(x, 0);
	return tree[x].val;
}
void insert(int v)//添加元素 
{
	int x = root, p = 0;
	while (x && tree[x].val != v)
	{
		p = x;
		x = tree[x].son[v > tree[x].val];
	}
	if (x) tree[x].cnt++;
	else
	{
		x = tree.size();
		tree[p].son[v > tree[p].val] = x;
		tree.emplace_back(v);
		tree[x].fa = p;
	}
	splay(x, 0);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	tree.emplace_back(0);
	tree[0].sz = 0, tree[0].cnt = 0;
	insert(-0x3f3f3f3f);
	insert(0x3f3f3f3f);
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		int op, x;
		cin >> op >> x;
		switch (op)
		{
		case 1:
			insert(x);
			break;
		case 2:
			del(x);
			break;
		case 3:
			cout << getrk(x) << '\n';
			break;
		case 4:
			cout << getval(x) << '\n';
			break;
		case 5:
			cout << tree[get_pre(x)].val << '\n';
			break;
		case 6:
			cout << tree[get_suc(x)].val << '\n';
			break;
		}
	}
	return 0;
}
