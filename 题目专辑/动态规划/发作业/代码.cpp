#include <bits/stdc++.h>

using i64 = long long;

void solve(void) {
    int n, m, w;
    std::cin >> n >> m >> w;
    std::vector<int> a(m + 1);
    for (int i = 1; i <= m; i++) std::cin >> a[i];

    auto get = [&](std::vector<int> &x) -> int {
        int res = 0;
        for (int i = 1; i <= w; i++) res |= (x[i] << (5 * (w - i)));
        return res;
    };

    std::vector<std::map<int, i64>> f(m + 1);
    auto dfs = [&](auto && self, int i, int c) -> i64 {
        if (f[i].count(c)) return f[i][c];
        std::vector<int> x(w + 1);
        for (int i = 1; i <= w; i++) x[i] = (c >> (5 * (w - i))) & 31;
        if (i == m) {
            return f[i][c] = *std::max_element(x.begin() + 1, x.end());
        }

        i64 ans = 1e18;
        int s = 0;
        for (int i = 1; i <= w; i++) {
            //if ((c >> (5 * i)) & 31) s++;
            if (x[i]) s++;
        }

        if (s < w) {
            std::vector<int> b(w + 1);
            for (int j = 1; j <= w; j++) b[j] = x[j];
            b[s + 1] = a[i + 1];
            std::sort(b.begin() + 1, b.end(), std::greater<int>());
            ans = std::min(ans, self(self, i + 1, get(b)));
        }

        for (int j = 1; j <= w; j++) {
            int d = x[j];
            if (d == 0) continue;
            if (j > 1 && x[j] == x[j - 1]) continue;
            std::vector<int> b(w + 1);
            int idx = 1;
            for (int k = 1; k <= w; k++) {
                if (x[k] > d) b[idx++] = x[k];
            }
            ans = std::min(ans, (i64)2 * d + self(self, i, get(b)));
        }

        return f[i][c] = ans;
    };

    std::cout << dfs(dfs, 0, 0);
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t = 1;
    //std::cin >> t;
    while (t--) solve();

    return 0;
}
