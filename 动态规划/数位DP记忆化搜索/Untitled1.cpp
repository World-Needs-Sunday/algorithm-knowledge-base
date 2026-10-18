#include <bits/stdc++.h>
using namespace std;
#define int long long
string l,r;
int dp[30][2][30][2],N[30];
int dqw;
int dfs(int w,int is_limited,int qianyiwei,int qdz){
    if(dp[w][is_limited][qianyiwei][qdz]!=-1) return dp[w][is_limited][qianyiwei][qdz];
    if(w==dqw+1) return 1;
    dp[w][is_limited][qianyiwei][qdz]=0;
    for(int i=0;i<10;i++){
        if(abs(qianyiwei-i)<2&&qdz!=1) continue;
        if(is_limited&&i>N[w]) continue;
        dp[w][is_limited][qianyiwei][qdz]+=dfs(w+1,is_limited&&(i==N[w]),i,qdz&&(i==0));
    }
    return dp[w][is_limited][qianyiwei][qdz];
}
int solve(string x){
	memset(dp,-1,sizeof dp);
	 for(int i=0;i<x.size();i++){
	           N[i+1]=x[i]-48;
	       }
	 dqw=x.size();
	 return dfs(1,1,0,1);
}
signed main(){
	int a,b;cin>>a>>b;a--;
	l=to_string(a),r=to_string(b);
	cout<<solve(r)-solve(l);
}
