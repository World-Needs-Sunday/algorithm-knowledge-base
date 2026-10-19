#include <bits/stdc++.h>
using namespace std;
int f[1010][1010];
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n;cin>>n;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=i;j++){
            cin>>f[i][j];
        }
    }
    for(int i=n-1;i>=1;i--){
        for(int j=1;j<=i;j++){
            f[i][j]+=max(f[i+1][j],f[i+1][j+1]);
        }
    }
    cout<<f[1][1];
    return 0;
}
