#include <bits/stdc++.h>
using namespace std;
pair<int,int> a[1010];
int f[1010];
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
        for(int j=a[i].first;j<=v;j++){
             f[j]=max(f[j],f[j-a[i].first]+a[i].second);
        }
    }
    cout<<f[v];
    return 0;
}
