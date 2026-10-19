#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) x&(-x)
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
//const int N=1e5+10;
//int cnt=0;
//int a[N];
inline int read();
inline void write(int x);
const int MAXN = 10005;
vector<int> G1[MAXN];  // 邻接表存储有向图
int dfn[MAXN], low[MAXN], scc[MAXN];  // scc[u]表示u所属的SCC编号
stack<int> st;
int idx = 0, tot = 0;  // 时间戳、SCC数量
bool vis[MAXN];  // 标记是否在栈中

void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    st.push(u);
    vis[u] = true;
    for (int v : G1[u]) {
        if (!dfn[v]) {  // v未访问
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (vis[v]) {  // v已访问且在栈中（回边）
            low[u] = min(low[u], dfn[v]);
        }
    }
    // 找到SCC的根
    if (dfn[u] == low[u]) {
        tot++;
        int v;
        do {
            v = st.top();
            st.pop();
            vis[v] = false;
            scc[v] = tot;
        } while (v != u);
    }
}
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
	//int n;cin>>n;
    return 0;
}
inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch< '0'||ch>'9'){
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9'){
        x=(x<<1)+(x<<3)+(ch^48);
        ch=getchar();
    }
    return x*f;
}
inline void write(int x){
    if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9) write(x/10);
    putchar(x%10+'0');
}
