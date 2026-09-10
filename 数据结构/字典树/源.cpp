#include<bits/stdc++.h>
using namespace std;
const int sz = 1e5 + 10;
const int new_cap = 128;
struct node
{
	int cnt;
	unordered_map<char, int> son;
	node() :cnt(0) {}
	int find(char ch)
	{
		auto it = son.find(ch);
		return it == son.end() ? -1 : it->second;
	}
	void add(char ch, int rt) { son.emplace(ch, rt); }
};

struct strtree
{
	//默认根节点为1
	int idx;
	vector<node> tree_arr;
	strtree(int big) :idx(1) { tree_arr.resize(big); }
	void insert(const string& str)
	{
		int cur = 1;
		for (char ch : str)
		{
			int nxt = tree_arr[cur].find(ch);
			if (nxt == -1)
			{
				if (idx >= tree_arr.size() - 1) tree_arr.insert(tree_arr.end(), new_cap, node{});
				nxt = ++idx;
				tree_arr[cur].add(ch, nxt);
			}
			++tree_arr[nxt].cnt;
			cur = nxt;
		}
	}
	int find(const string& str)
	{
		int cur = 1;
		for (char ch : str)
		{
			cur = tree_arr[cur].find(ch);
			if (cur == -1) return 0;
		}
		return tree_arr[cur].cnt;
	}

	void clear()
	{
		for (int i = idx; i >= 0; i--)
		{
			tree_arr[i].son.clear();
			tree_arr[i].cnt = 0;
		}
		idx = 1;
	}
};

strtree tree(sz);
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n, q;
		cin >> n >> q;
		tree.clear();
		for (int i = 1; i <= n; i++)
		{
			string str;
			cin >> str;
			tree.insert(str);
		}
		for (int i = 1; i <= q; i++)
		{
			string str;
			cin >> str;
			cout << tree.find(str) << '\n';
		}
	}
	return 0;
}
