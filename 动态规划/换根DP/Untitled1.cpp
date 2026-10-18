#include<bits/stdc++.h>
using namespace std;
#define int long long
vector<int> G1[1000010];
int n,ans=0,jd=1;
int sz[1000010],f[1000010],dp[1000010];
void dfs1(int st,int pa){
	sz[st]=1,dp[st]=0;
	for(auto i:G1[st]){
		if(i==pa) continue;
		dfs1(i,st);
		sz[st]+=sz[i];
		dp[st]+=dp[i]+sz[i];
	}
}
void dfs2(int st,int pa){
	sz[st]=1,dp[st]=0;
	for(auto i:G1[st]){
		if(i==pa) continue;
		f[i]=f[st]+n-2*sz[i];
		dfs2(i,st);
	}
}
signed main(){
    cin>>n;
    for(int i=1;i<n;i++){
		int u,v;cin>>u>>v;
		G1[u].push_back(v);
		G1[v].push_back(u);
	}
	dfs1(1,-1);
	dfs2(1,-1);
	for(int i=1;i<=n;i++){
		if(ans<f[i]){ans=max(ans,f[i]);jd=i;}
	}
	cout<<jd;
    return 0;
}
