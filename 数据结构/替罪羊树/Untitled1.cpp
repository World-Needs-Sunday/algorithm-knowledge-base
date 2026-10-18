#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=4e5+10;
const double ALPHA=0.75;
int tot=0,rot=0;
struct bst{
    int u,cnt;
    int ls,rs;
    int sz;
    bst():u(0),cnt(0),ls(0),rs(0),sz(0){}
    bst(int b,int x):u(b),cnt(x),ls(0),rs(0),sz(1){}
} wcj[N];
int cxsearch(int b,int v){
    if(wcj[b].u==0||wcj[b].cnt==v) return wcj[b].u;
    if(v<wcj[b].cnt) return cxsearch(wcj[b].ls,v);
    else return cxsearch(wcj[b].rs,v);
}
bool isping(int b){           //判断子树是否平衡
    if(b==0) return true;
    int lsz=wcj[wcj[b].ls].sz;
    int rsz=wcj[wcj[b].rs].sz;
    return max(lsz,rsz)<=ALPHA*wcj[b].sz;
}
void findnode(int b,vector<int>& son){//中序遍历收集节点编号
    if(b==0) return;
    findnode(wcj[b].ls,son);
    son.push_back(b);
    findnode(wcj[b].rs,son);
}
int build(vector<int>& son,int l,int r){//重建平衡树
    if(l>r) return 0;
    int mid=(l+r)>>1;
    int nwr=son[mid];
    wcj[nwr].ls=build(son,l,mid-1);
    wcj[nwr].rs=build(son,mid+1,r);
    wcj[nwr].sz=wcj[wcj[nwr].ls].sz+wcj[wcj[nwr].rs].sz+1;
    return nwr;
}
void rebuild(int &b){              //重构子树
    vector<int>son;
    findnode(b,son);
    for(int id:son){
        wcj[id].ls=wcj[id].rs=0;
    }
    b=build(son,0,son.size()-1);
}
int crinsert(int b,int v){
    if(wcj[b].u==0){
        tot++;
        wcj[tot]=bst(tot,v);
        return wcj[tot].u;
    }
    if(v<wcj[b].cnt) wcj[b].ls=crinsert(wcj[b].ls,v);
    else wcj[b].rs=crinsert(wcj[b].rs,v);
    wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
    if(!isping(b)) rebuild(b);
    return wcj[b].u;
}
int scdelete(int b,int v){
    if(wcj[b].u==0) return 0;
    if(v<wcj[b].cnt){
        wcj[b].ls=scdelete(wcj[b].ls,v);
        wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
        if(!isping(b)) rebuild(b);
    }
    else if(v>wcj[b].cnt){
        wcj[b].rs=scdelete(wcj[b].rs,v);
        wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
        if(!isping(b)) rebuild(b);
    }
    else{
        if(wcj[b].ls==0){
            return wcj[b].rs;
        }
        else if(wcj[b].rs==0){
            return wcj[b].ls;
        }
        else{
            int nw=wcj[b].rs;
            while(wcj[nw].ls!=0) nw=wcj[nw].ls;
            wcj[b].cnt=wcj[nw].cnt;
            wcj[b].rs=scdelete(wcj[b].rs,wcj[nw].cnt);
            wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
            if(!isping(b)) rebuild(b);
        }
    }
    return wcj[b].u;
}
int queryless(int b,int x){
    if(b==0) return 0;
    if(x<=wcj[b].cnt) return queryless(wcj[b].ls,x);
    else return wcj[wcj[b].ls].sz+1+queryless(wcj[b].rs,x);
}
int kth(int b,int k){
    if(k<=wcj[wcj[b].ls].sz) return kth(wcj[b].ls,k);
    else if(k==wcj[wcj[b].ls].sz+1) return wcj[b].cnt;
    else return kth(wcj[b].rs,k-wcj[wcj[b].ls].sz-1);
}
int qianququ(int b,int x){
    int ans=LLONG_MIN;
    while(b!=0){
        if(wcj[b].cnt<x){
            ans=wcj[b].cnt;
            b=wcj[b].rs;
        }else{
            b=wcj[b].ls;
        }
    }
    return ans;
}
int houjiji(int b,int x){
    int ans=LLONG_MAX;
    while(b!=0){
        if(wcj[b].cnt>x){
            ans=wcj[b].cnt;
            b=wcj[b].ls;
        }else{
            b=wcj[b].rs;
        }
    }
    return ans;
}
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    int n;cin>>n;
    while(n--){
        int opt,x;cin>>opt>>x;
        if(opt==1) rot=crinsert(rot,x);
        else if(opt==2) rot=scdelete(rot,x);
        else if(opt==3) cout<<queryless(rot,x)+1<<'\n';
        else if(opt==4) cout<<kth(rot,x)<<'\n';
        else if(opt==5) cout<<qianququ(rot,x)<<'\n';
        else if(opt==6) cout<<houjiji(rot,x)<<'\n';
    }
    return 0;
}
