#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=15,M=(1<<12);
int n,m,cnt;
int cot[M][M],dp[N][M],G1[N][N];
inline int read();
inline void write(int x);
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    cin>>n>>m;

    memset(G1,0x3f,sizeof G1);
    memset(dp,0x3f,sizeof dp);
    int inf=dp[0][0];
    cnt=inf;
    if(n==1){
        cout<<0;return 0;
    }
    for(int i=1;i<=m;i++){
        int u,v,w;cin>>u>>v>>w;
        G1[u][v]=min(G1[u][v],w);
        G1[v][u]=min(G1[v][u],w);
    }
    for(int i=1;i<(1<<n);i++){
        for(int j=i;j;j=(j-1)&i){
            if(j==i) continue;
            for(int t=1;t<=n;t++){
                int tmp=inf;
                if(!(((i^j)>>(t-1))&1)) continue;
                for(int l=1;l<=n;l++){
                    if((j>>(l-1))&1) tmp=min(tmp,G1[l][t]);
                }
                if(tmp>=inf){
                    cot[j][i]=inf;break;
                }else cot[j][i]+=tmp;
            }
        }
    }
    for(int u=1;u<=n;u++){
        memset(dp,0x3f,sizeof dp);
        dp[0][1<<(u-1)]=0;
        for(int i=1;i<n;i++){
            for(int j=1;j<(1<<n);j++){
                for(int k=j;k;k=(k-1)&j){
                    if(j==k) continue;
                    if(cot[k][j]!=inf) dp[i][j]=min(dp[i][j],dp[i-1][k]+i*cot[k][j]);
                }
            }
            cnt=min(cnt,dp[i][(1<<n)-1]);
        }
    }
    cout<<cnt;
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
