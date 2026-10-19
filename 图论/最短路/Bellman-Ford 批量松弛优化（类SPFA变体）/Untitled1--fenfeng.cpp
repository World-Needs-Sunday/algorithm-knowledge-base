#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 2010;
vector<pair<int,int>> e[N];
int dis[N], cnt[N], vis[N];
queue<int> q;
bool spfa(int n) {
    for(int i=1;i<=n;i++){
        dis[i] = 0, vis[i] = 1;
          q.push(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop(), vis[u] = 0;
        for (auto ed : e[u]) {
              int v = ed.first, w = ed.second;
              if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;  
                if (cnt[v]>=n) return false;
                if (!vis[v]) q.push(v), vis[v] = 1;
            }
        }
    }
    return true;
}
signed main(){
    int n,m;cin >> n >> m;
    memset(dis,0x3f,sizeof dis);
    for(int i = 1; i <= m; i ++) {
        int a, b; cin >> a >> b;
        int c; cin >> c;
        e[a].push_back({b, c});
    }

    if(spfa(n)) cout<<"No";
    else cout<<"Yes";    
    return 0;
}
