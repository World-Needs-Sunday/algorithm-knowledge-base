#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=1e5+10;
int n;
int fa[N],dep[N];
vector<int> G[N],F[N];
int ans[N];
bool cmp1(int a,int b){
    return F[a][0]>F[b][0];
}
multiset<int> zyh;
void dfs1(int x){
    dep[x]=dep[fa[x]]+1;
    int sizg=G[x].size();
    if(sizg==0){
        F[x].push_back(0);
        return;
    }
    for(auto i:G[x]) dfs1(i);
    sort(G[x].begin(),G[x].end(),cmp1);
    vector<int> maq,mah;
    for(int i=0,j=sizg-1;i<sizg&&j>=0;i++,j--){
        if(i==0) maq.push_back(max(0ll,F[G[x][i]][0]+1+2*(i))),mah.push_back(max(0ll,F[G[x][j]][0]+1+2*(j)));
        else maq.push_back(max(maq[i-1],F[G[x][i]][0]+1+2*(i))),mah.push_back(max(mah[i-1],F[G[x][j]][0]+1+2*(j)));
    }
    F[x].push_back(maq[sizg-1]);
    F[x].push_back(mah[sizg-2]-2);
    for(int i=1;i<sizg-1;i++){
        F[x].push_back(max(maq[i-1],mah[sizg-i-2]-2));
    }
    if(sizg-1!=0) F[x].push_back(maq[sizg-2]);
}
void dfs2(int x){
    if(!zyh.empty()){
        auto tmp=zyh.end();tmp--;
        ans[x]=max(ans[x],*tmp+dep[x]);
    }
    vector<int> son;
    for(auto i:G[x]){
        if(G[i].size()>0) son.push_back(i);
    }
    for(int i=0;i<(int)son.size();i++){
        ans[x]=max(ans[x],2*i+1+F[son[i]][0]);
    }
    for(int i=0;i<(int)G[x].size();i++){
        if(G[x].size()>1) zyh.insert(F[x][i+1]-dep[x]);
        dfs2(G[x][i]);
        if(G[x].size()>1) zyh.extract(F[x][i+1]-dep[x]);
    }
    if((int)G[x].size()+dep[x]==n){
        ans[x]=0;
    }
}
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    cin>>n;
    fa[1]=0;
    for(int i=2;i<=n;i++){
        cin>>fa[i];
        G[fa[i]].push_back(i);
    }
    dfs1(1);
    dfs2(1);
    for(int i=1;i<=n;i++) cout<<ans[i]<<" ";
    return 0;
}
