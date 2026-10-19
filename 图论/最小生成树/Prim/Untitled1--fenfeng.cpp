#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
const int MaxN = 1e6 + 10;
int n,m,cnt=0,ans=0;
vector<pair<int,int>> G1[MaxN];
vector<int> d, vis;
        struct Point{
            int id,x;
            bool operator<(const Point &aa)const{return x>aa.x;}
        };
        void solve(int S) {
            d.assign(1e6,INF);
            vis.assign(1e6,0);
            d[S]=0;
            priority_queue<Point> q;
            q.push({S, 0});

            while (!q.empty()) {

                auto it= q.top(); q.pop();
                if (vis[it.id]) continue;
                vis[it.id] = 1;
                cnt+=it.x;
                //cout<<cnt<<endl;
                if(ans>=n)break;
                ans++;
                for (auto t: G1[it.id]) {
                    if (t.second < d[t.first]) {
                        d[t.first] = t.second;
                        if (!vis[t.first]){ q.push({t.first, d[t.first]});}
                    }
                }
            }
        }

int main() {
    cin >> n >> m;
    for(int i = 1; i <= m; i ++) {
        int a, b; cin >> a >> b;
        int c; cin >> c;
        G1[a].push_back({b, c});
        G1[b].push_back({a, c});
    }
    solve(1);
    if(ans!=n)cout << "impossible" << endl;
    else cout << cnt << endl;
    return 0;
}
