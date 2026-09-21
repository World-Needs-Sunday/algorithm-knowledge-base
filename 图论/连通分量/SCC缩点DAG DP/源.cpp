#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int tim, col_cnt, sk_top;
vector<bool> in_sk;
vector<int> dfn, low, sk, col, in_rt,SCC_w,W;
vector<vector<int>> edges, col_arr, SCC_edges;
void add(int x, int y)
{
    edges[x].emplace_back(y);
}
void mark(int rt)
{
    dfn[rt] = low[rt] = tim++;
    sk[sk_top++] = rt;
    in_sk[rt] = true;
}
void init(int n)
{
    W.assign(n + 1, 0);
    SCC_w.assign(1, 0);
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    edges.assign(n + 1, vector<int>());
    sk.assign(n + 5, 0);
    in_sk.assign(n + 1, false);
    in_rt.assign(n + 1, false);
    col_arr.emplace_back();
    col.assign(n + 1, 0);
    tim = 1;
    col_cnt = 0;
    sk_top = 0;
}
void tarjan(int rt)
{
    mark(rt);
    for (int i : edges[rt])
    {
        if (!dfn[i])
        {
            tarjan(i);
            low[rt] = min(low[rt], low[i]);
        }
        else if (in_sk[i]) low[rt] = min(low[rt], dfn[i]);
    }
    if (dfn[rt] == low[rt])
    {
        col_arr.emplace_back();
        SCC_w.emplace_back();
        ++col_cnt;
        while (in_sk[rt])
        {
            int tmp = sk[sk_top - 1];
            col_arr[col_cnt].emplace_back(tmp);
            col[tmp] = col_cnt;
            in_sk[tmp] = false;
            SCC_w[col_cnt] += W[tmp];
            sk_top--;
        }
    }
}
void SCC()
{
    in_rt.assign(col_cnt + 1, 0);
    SCC_edges.assign(col_cnt + 1, vector<int>());
    vector<int> vis(col_cnt + 1, 0);
    for (int u = 1; u <= col_cnt; u++)
    {
        for (int i : col_arr[u])
        {
            for (int j : edges[i])
            {
                int v = col[j];
                if (vis[v] == u || u == v) continue;
                vis[v] = u;
                SCC_edges[u].emplace_back(v);
                in_rt[v]++;
            }
        }
    }
}

int fn(int n)
{
    int head = 0, tail = 0;
    for (int i = 1; i <= n; i++)
    {
        if (!dfn[i]) tarjan(i);
    }
    vector<int> deq(col_cnt + 2, 0);
    vector<int> A(col_cnt + 2, 0);
    SCC();
    int ans = 0;
    for (int i = 1; i <= col_cnt; i++)
    {
        if (!in_rt[i])
        {
            deq[tail++] = i;
            A[i] = SCC_w[i];
            ans = max(ans, SCC_w[i]);
        }
    }
    
    while (head < tail)
    {
        int i = deq[head++];
        for (int j : SCC_edges[i])
        {
            in_rt[j]--;
            A[j] = max(A[i] + SCC_w[j], A[j]);
            if (!in_rt[j])
            {
                deq[tail++] = j;
                ans = max(ans, A[j]);
            }
        }
    }
    return ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n, m;
    cin >> n >> m;
    init(n);
    for (int i = 1; i <= n; i++) cin >> W[i];
    for (int i = 1; i <= m; i++)
    {
        int x, y;
        cin >> x >> y;
        add(x, y);
    }
    cout << fn(n);
    return 0;
}