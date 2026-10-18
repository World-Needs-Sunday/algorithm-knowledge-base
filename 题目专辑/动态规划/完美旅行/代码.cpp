#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=3e5+10,M=23,mod=998244353;
int n,m,k;
vector<int> G8[N];
PII a[N];
int fh[N],mac[N];
int dp[(1<<M)];
int jcr[4*N],nyj[4*N];
int ksm(int x,int y,int p){//快速幂
    int res=1;
    if(y==0) return 1;
    if(y&1) res=x*ksm(x,y-1,p)%p;
    else res=ksm(x,y/2,p),res=res*res%p;
    return res;
}
void jiecycl(int n){
    jcr[0]=1;
    for(int i=1;i<=n;i++){
        jcr[i]=jcr[i-1]*i%mod;
    }
    nyj[n]=ksm(jcr[n],mod-2,mod);
    for(int i=n-1;i>=0;i--){
        nyj[i]=nyj[i+1]*(i+1)%mod;
    }
}
int cabz(int n,int k){//组合数
    if(k<0||k>n) return 0;
    return jcr[n]*nyj[k]%mod*nyj[n-k]%mod;
}
void dfs(int x,int fa){
    fh[x]=fh[fa];
    for(int i=1;i<=m;i++){
        if(a[k+i]==(PII){x,fa}||a[k+i]==(PII){fa,x}) fh[x]|=1<<(i-1);
    }
    for(auto i:G8[x]){
        if(i==fa) continue;
        dfs(i,x);
    }
}
inline int read();
inline void write(int x);
PII sb010[N];
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    cin>>n>>m>>k;
    jiecycl(2*(n+k));
    for(int i=1;i<n;i++){
        int u,v;cin>>u>>v;
        G8[u].push_back(v);
        G8[v].push_back(u);
        sb010[i]={u,v};
    }
    for(int i=1;i<=m;i++){
        int x;cin>>x;
        a[k+i]=sb010[x];
    }
    for(int i=1;i<=k;i++){
        int u,v;cin>>u>>v;
        a[i]={u,v};
    }
    dfs(1,0);
    for(int i=1;i<=k;i++){
        mac[i]=fh[a[i].fi]^fh[a[i].se];
        dp[mac[i]]++;
    }
    for(int i=0;i<m;i++){
        for(int sta=(1<<m)-1;sta>=0;sta--){
            if(sta&(1<<i)) dp[sta]+=dp[sta^(1<<i)];
        }
    }
    for(int i=1;i<=m;i++){
        int ans=0;
        for(int sta=0;sta<=(1<<m)-1;sta++){
            int ct=__builtin_popcount(sta);
            if((ct&1)==(m&1)) ans=(ans%mod+cabz(dp[sta],i)%mod+mod)%mod;
            else ans=(ans%mod-cabz(dp[sta],i)%mod+mod)%mod;
        }
        ans%=mod;
        if(ans!=0){
            cout<<i<<" "<<ans;
            exit(0);
        }
    }
    cout<<-1;
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
