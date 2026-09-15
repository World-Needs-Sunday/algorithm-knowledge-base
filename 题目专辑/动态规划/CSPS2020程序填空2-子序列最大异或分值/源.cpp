#include <iostream>
using namespace std;
typedef long long LL;

const int MAXN = 40000, M = 16, B = M >> 1, MS = (1 << B) - 1;
const LL INF = 1000000000000000LL;

LL Max[MS + 4][MS + 4];                 // Max[高8位][低8位]

int w(int x) {                          // 统计 x 二进制中 1 的个数
    int s = 0;
    while (x) {
        x &= x - 1;                     // ① 去掉最低位的 1
        s++;
    }
    return s;
}

void to_max(LL& x, LL y) {              // 如果 x < y，将 x 改为 y
    if (x < y) x = y;
}

int main() {
    int n;
    LL ans = 0;
    cin >> n;                                           // 输入 n
    for (int x = 0; x <= MS; x++)
        for (int y = 0; y <= MS; y++)
            Max[x][y] = -INF;                           // 初始化 Max 矩阵为 -∞
    for (int i = 1; i <= n; i++) {
        LL a;
        cin >> a;                                       // 输入 n 个 a
        int x = a >> B, y = a & MS;                     // ② x=高8位，y=低8位
        LL v = 0;                                       // ③ 单独成一个子序列，分值 0
        for (int z = 0; z <= MS; z++)                   // 枚举前一个数的高 8 位 z
            to_max(v, Max[z][y] + w(x ^ z));            // ④ 转移
        for (int z = 0; z <= MS; z++)                   // 枚举"下一个数"的低 8 位 z
            to_max(Max[x][z], v + w(y ^ z));            // ⑤ 更新 DP 表
        to_max(ans, v);
    }
    cout << ans << endl;
    return 0;
}
