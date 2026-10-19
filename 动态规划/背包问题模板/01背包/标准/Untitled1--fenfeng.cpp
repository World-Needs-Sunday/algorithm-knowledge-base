#include <bits/stdc++.h>
using namespace std;
pair<int,int> a[1010];
int f[1010][1010],w[1010];
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n,v;cin>>n>>v;
    for(int i=1;i<=n;i++){
        cin>>a[i].first>>a[i].second;
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=v;j++){
            if(a[i].first>j) f[i][j]=f[i-1][j];
            else f[i][j]=max(f[i-1][j],f[i-1][j-a[i].first]+a[i].second);
        }
    }
    cout<<f[n][v];
    return 0;
}
