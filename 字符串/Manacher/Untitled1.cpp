#include<bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    // 插入分隔符
    string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    int m = t.size();

    vector<int> p(m, 0);
    int C = 0, R = 0;
    int ans = 0;

    for (int i = 0; i < m; i++) {
        // 镜像继承
        if (i < R) p[i] = min(p[2 * C - i], R - i);

        // 暴力扩展
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < m && t[i - p[i] - 1] == t[i + p[i] + 1])
            p[i]++;

        // 更新最右回文串
        if (i + p[i] > R) {
            C = i;
            R = i + p[i];
        }

        ans = max(ans, p[i]);
    }

    cout << ans << '\n';
    return 0;
}
