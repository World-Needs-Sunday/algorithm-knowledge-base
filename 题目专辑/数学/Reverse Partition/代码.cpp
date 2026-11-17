#include<bits/stdc++.h>
using namespace std;
const long long mod = 1e9 + 7;
const int MAXN = 200005;
long long fact[MAXN], invfact[MAXN];

long long fastpow(long long a, long long b, long long q)
{
    if (b == 0) return 1;
    long long ans = 1;
    long long gap = a;
    while (b)
    {
        if (b & 1) ans = (ans * gap) % q;
        gap = (gap * gap) % q;
        b >>= 1;
    }
    return ans;
}

void init()
{
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++)
        fact[i] = fact[i - 1] * i % mod;
    invfact[MAXN - 1] = fastpow(fact[MAXN - 1], mod - 2, mod);
    for (int i = MAXN - 2; i >= 0; i--)
        invfact[i] = invfact[i + 1] * (i + 1) % mod;
}

long long fn(long long s, long long k)
{
    if (s < 0 || k < 0 || k > s) return 0;
    int n = (int)s, r = (int)k;
    return fact[n] * invfact[r] % mod * invfact[n - r] % mod;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    init();

    int T;
    cin >> T;
    while (T--)
    {
        int n, m, q;
        cin >> n >> m >> q;

        vector<long long> a(n + 1, 0);
        for (int i = 1; i <= n; i++)
        {
            cin >> a[i];
            a[i] += a[i - 1];
        }

        vector<long long> D(n + 1, 0);
        map<long long, vector<int>> mp; 
        vector<int> pref0(n + 1, 0); 

        for (int x = 1; x <= n - 1; x++)
        {
            D[x] = a[x] + a[n - x] - a[n];
            mp[D[x]].push_back(x);
            pref0[x] = pref0[x - 1] + (D[x] == 0);
        }

        int K = pref0[n - 1];

        auto countZero = [&](int L, int R) -> int {
            if (L > R) return 0;
            return pref0[R] - pref0[L - 1];
            };

        auto countVal = [&](long long val, int L, int R) -> int {
            if (L > R) return 0;
            auto it = mp.find(val);
            if (it == mp.end()) return 0;
            const auto& vec = it->second;
            return upper_bound(vec.begin(), vec.end(), R)
                - lower_bound(vec.begin(), vec.end(), L);
            };

        while (q--)
        {
            int i;
            long long v;
            cin >> i >> v;

            long long newK = K;

            if (v != 0)
            {
                if (2LL * i <= n)
                {
                    int L = i, R = n - i;
                    newK -= countZero(L, R);
                    newK += countVal(-v, L, R);
                }
                else
                {
                    int L = n - i + 1, R = i - 1;
                    newK -= countZero(L, R);
                    newK += countVal(v, L, R);
                }
            }

            cout << fn(newK, m - 1) << '\n';
        }
    }
    return 0;
}