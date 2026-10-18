#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=510,mod=1e9+7;
int cnt=0,n,k;
string s;
int sr[N][N],b[N][N],f[N][N],h[N][N],c[N][N],tr[N][N];
bool cmk(int l,int r){
	if((s[l]=='('||s[l]=='?')&&(s[r]==')'||s[r]=='?')) return true;
	else return false;
}
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    cin>>n>>k;
    cin>>s;s=' '+s;
    for(int i=1;i<=n;i++){
        //cout<<s[i];
        sr[i][i-1]=1;
    }
	for(int len=1;len<=n;len++){
		for(int l=1;l<=n-len+1;l++){
			int r=l+len-1;
            //if(len<=k&&sr[l+1][r]==1&&(s[l]=='*'||s[l]=='?')) sr[l][r]=1;
			if(len<=k&&sr[l][r-1]==1&&(s[r]=='*'||s[r]=='?')) sr[l][r]=1;
			if(len>=2){
                bool u=cmk(l,r);
                //cout<<u<<" "<<l<<" "<<r<<"\n";
                if(u) b[l][r]=(sr[l+1][r-1]%mod+f[l+1][r-1]%mod+h[l+1][r-1]%mod+c[l+1][r-1]%mod)%mod;
			    for(int i=l;i<r;i++){
				    f[l][r]=(f[l][r]%mod+h[l][i]*sr[i+1][r]%mod)%mod;
				    h[l][r]=(h[l][r]%mod+(f[l][i]+h[l][i])*b[i+1][r]%mod)%mod;
    				c[l][r]=(c[l][r]%mod+(c[l][i]+tr[l][i])*b[i+1][r]%mod)%mod;
				    tr[l][r]=(tr[l][r]%mod+c[l][i]*sr[i+1][r]%mod)%mod;
    			}
            }
            h[l][r]=(h[l][r]%mod+b[l][r]%mod)%mod;
			tr[l][r]=(tr[l][r]%mod+sr[l][r]%mod)%mod;
		}
    }
    cout<<h[1][n];
	// for(int len=1;len<=n;len++){
 //        for(int l=1;l+len-1<=n;l++){
 //            int r=len+l-1;
 //            cout<<l<<" "<<r<<" "<<sr[l][r]<<" "<<b[l][r]<<" "<<f[l][r]<<" "<<h[l][r]<<" "<<c[l][r]<<" "<<tr[l][r]<<"\n";
 //        }
 //    }
	return 0;
}
