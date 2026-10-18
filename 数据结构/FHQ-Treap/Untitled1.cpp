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
int tot=0,rot=0;
auto seed=chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rng(seed);
int rdm[N];
struct bst{
	int u,cnt;
	int ls,rs;
	int sz;
	int cho,prw;
	bst():u(0),cnt(0),ls(0),rs(0),sz(0),cho(0),prw(0){}
	bst(int b,int x):u(b),cnt(x),ls(0),rs(0),sz(1),cho(1),prw(rdm[tot]){}
} wcj[N];
int nwn(int v) {
    tot++;
    wcj[tot]=bst(tot,v);
    return tot;
}
void pulp(int u){
	wcj[u].sz=wcj[wcj[u].ls].sz+wcj[wcj[u].rs].sz+wcj[u].cho;
}
void split(int u,int v,int &x,int &y){
	if(!u){
		x=y=0;return ;
	}
	if(wcj[u].cnt<=v){
		x=u;split(wcj[u].rs,v,wcj[u].rs,y);pulp(x);
	}else{
		y=u;split(wcj[u].ls,v,x,wcj[u].ls);pulp(y);
	}
}
int merge(int x,int y){
	if(!x||!y) return x|y;
	if(wcj[x].prw<wcj[y].prw){
		wcj[x].rs=merge(wcj[x].rs,y);pulp(x);return x;
	}else{
		wcj[y].ls=merge(x,wcj[y].ls);pulp(y);return y;
	}
}
int cxsearch(int b,int v){
	if(wcj[b].u==0||wcj[b].cnt==v) return wcj[b].u;//返回下标
	if(v<wcj[b].cnt) return cxsearch(wcj[b].ls,v);
	else return cxsearch(wcj[b].rs,v);
}
int crinsert(int v){
	int a,b,c;
	split(rot,v,b,c);
	split(b,v-1,a,b);
	if(b==0) return merge(merge(a,nwn(v)),c);
	else wcj[b].cho++;
	pulp(b);
	return merge(merge(a,b),c);;
}
int scdelete(int v){
	int a,b,c;
	split(rot,v,b,c);
	split(b,v-1,a,b);
	if(b==0) return rot;
	else if(wcj[b].cho==1){
		return merge(merge(a,merge(wcj[b].ls,wcj[b].rs)),c);
	} 
	else wcj[b].cho--;
	pulp(b);
	return merge(merge(a,b),c);
}
// 返回树中值 < x 的节点个数
int queryless(int b,int x){
    if(b==0) return 0;
    if(x<=wcj[b].cnt){
        return queryless(wcj[b].ls,x);
    }else{
        return wcj[wcj[b].ls].sz+wcj[b].cho+queryless(wcj[b].rs,x);
    }
}
// 返回第 k 小的值
int kth(int b,int k){
    if(k<=wcj[wcj[b].ls].sz){
        return kth(wcj[b].ls,k);
    }else if (k<=wcj[wcj[b].ls].sz+wcj[b].cho){
        return wcj[b].cnt;
    }else{
        return kth(wcj[b].rs,k-wcj[wcj[b].ls].sz-wcj[b].cho);
    }
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
	//主函数实现的是P3369
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    int n;cin>>n;
    for(int i=1;i<=n;i++) rdm[i]=rng(); 
    while(n--){
		int opt,x;cin>>opt>>x;
		if(opt==1) rot=crinsert(x);
		else if(opt==2) rot=scdelete(x);
		else if(opt==3) cout<<queryless(rot,x)+1<<'\n';
		else if(opt==4) cout<<kth(rot,x)<<'\n';
		else if(opt==5) cout<<qianququ(rot,x)<<"\n";
		else if(opt==6) cout<<houjiji(rot,x)<<'\n';
	}
    return 0;
}
