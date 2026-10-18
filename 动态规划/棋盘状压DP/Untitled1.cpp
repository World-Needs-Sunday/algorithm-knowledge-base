#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
//const int N=1e5+10;
//int cnt=0;
//int a[N];
int mp[110];
int dp[3][1<<11][1<<11];
int ndp[110][1<<11];
vector<int> pas[110];
inline int read();
inline void write(int x);
int popcount(int x){
    return __builtin_popcount(x);
}
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    
    int n,m;cin>>n>>m;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            char c;cin>>c;
            if(c=='H') mp[i]|=(1<<(m-j));
        }
    }
    for(int i=1;i<=n;i++){
        for(int j=0;j<(1<<m);j++){
                //cout<<dp[i][j]<<' ';
                //if(i!=1) dp[i][j]=LLONG_MIN;
            if((((j<<2)&j)>0)||(((j<<1)&j)>0)||(((j>>2)&j)>0)||(((j>>1)&j)>0)) continue;
            if(j&mp[i]) continue;
            pas[i].push_back(j);
        }
           //cout<<endl;
    }
    pas[0].push_back(0);
    for(int i=0;i<=2;i++){
        for(int j=0;j<(1<<m);j++){
            for(int u=0;u<(1<<m);u++){
                dp[i][j][u]=LLONG_MIN;
            }
        }
    }
    for(int j:pas[1]){
        dp[1][j][0]=popcount(j);
    }
    for(int i=2;i<=n;i++){
        for(int j:pas[i]){
            for(int u:pas[i-1]){
                if(u&j) continue;
                for(int o:pas[max(i-2,0ll)]){
                    if(o&u) continue;
                    if(o&j) continue;
                    
                    dp[i%3][j][u]=max(dp[i%3][j][u],dp[(i-1)%3][u][o]+popcount(j));
                }
                
            }
        }
    }
    
    int cnt=0;
    for(int j:pas[n]){
        for(int k:pas[n-1]){
            cnt=max(cnt,dp[n%3][j][k]);
        }
    }
       cout<<cnt<<endl;
    
    
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
