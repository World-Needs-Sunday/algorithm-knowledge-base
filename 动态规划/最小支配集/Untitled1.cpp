#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> bo[10010];
int f[10010][3],v[10010];
void dp(int x, int fa){
    f[x][0]=1;f[x][1]=0;f[x][2]=0;
    bool buxiao=true;
	int yanglaojin = 10000000;
    for(int i=0;i<bo[x].size();i++){
        int y=bo[x][i];
        if (y == fa) continue;
        //cout<<y<<" "<<x<<endl;
        dp(y, x);//TODO
        f[x][0]+=min({f[y][0],f[y][1],f[y][2]});
        f[x][2]+=min(f[y][0],f[y][1]);
        if(f[y][0]<=f[y][1]) buxiao=false;
        f[x][1]+=min(f[y][0],f[y][1]);
        yanglaojin=min(yanglaojin,f[y][0]-f[y][1]);
    }
    if(buxiao) f[x][1]+=yanglaojin;
}
int main(){
	cin>>n;

	for(int i=1;i<n;i++){
		int s;cin>>s;
		
		int b;cin>>b;
		bo[s].push_back(b);
		bo[b].push_back(s);
	}

    dp(1, -1);
    cout<<min(f[1][0],f[1][1])<<endl;

    return 0;
}
