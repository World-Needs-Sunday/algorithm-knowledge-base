#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) 
    {
        int n, k;
        cin >> n >> k;
        vector<long long> a(n + 1);
        long long sum = 0;
        for (int i = 1; i <= n; i++) 
        {
            cin >> a[i];
            sum += a[i];
        }
 
        long long tmp = 0;
 
        int L = min(k - 1, n - k + 1);
        int R = max(k, n - k + 2);
 
        if (n - k + 2 <= k - 1) 
        {
            for (int i = n - k + 2; i <= k - 1; i++) tmp += a[i];
        }
 
        int len = L;
        for (int i = 0; i < len; i++) 
        {
            int l = L - i;     
            int r = R + i; 
            tmp += min(a[l], a[r]);
        }
 
        long long ans = sum - tmp;
        cout << ans << '\n';
    }
    return 0;
}