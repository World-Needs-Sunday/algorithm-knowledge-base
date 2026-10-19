#include <bits/stdc++.h>
using namespace std;
const int INF=1e9;
vector<vector<int>> f(310,vector<int>(310,INF));
vector<int> a(310),p(310);
int main(){
    int n;cin>>n;
    for(int i=1;i<=n;i++){
        cin>>a[i];f[i][i]=0;
        p[i]=p[i-1]+a[i];
    }
    for(int d=2;d<=n;d++){
        for(int l=1;l<=n;l++){
            int r=l+d-1;
            if(r>n) continue;
            for(int k=l;k<r;k++){
                f[l][r]=min(f[l][r],f[l][k]+f[k+1][r]+p[r]-p[l-1]);
            }
        }
    }
    cout<<f[1][n];
}
