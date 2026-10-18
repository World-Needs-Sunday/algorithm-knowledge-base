#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=2e3+10;
int cnt=0,n,k;
vector<PII> G1[N];
int siz[N],dp[N][N];
void dfs1(int x,int fa){
	siz[x]=1;
	for(auto i:G1[x]){
		if(i.fi==fa) continue;
		dfs1(i.fi,x);
		siz[x]+=siz[i.fi];
	}
}
void dfs2(int x,int fa){
	dp[x][0]=dp[x][1]=0;
	for(auto j:G1[x]){
		if(j.fi==fa) continue;
        dfs2(j.fi,x);
		for(int m=min(k,siz[x]);m>=0;m--){
			if(dp[x][m]!=-1) dp[x][m]+=dp[j.fi][0]+siz[j.fi]*(n-k-siz[j.fi])*j.se;
			for(int v=min(m,siz[j.fi]);v>0;v--){
				if(dp[x][m-v]==-1) continue;
				int tot=v*(k-v)+(siz[j.fi]-v)*(n-k-siz[j.fi]+v);
				dp[x][m]=max(dp[x][m],dp[x][m-v]+dp[j.fi][v]+tot*j.se);
			}
		}		
	}
}
inline int read();
inline void write(int x);
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    memset(dp,-1,sizeof dp);
	cin>>n>>k;
    if(n-k<k) k=n-k;
	for(int i=1;i<n;i++){
		int u,v,w;cin>>u>>v>>w;
		G1[u].push_back({v,w});
		G1[v].push_back({u,w});
	}
	dfs1(1,0);
	dfs2(1,0);
	cout<<dp[1][k];
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
