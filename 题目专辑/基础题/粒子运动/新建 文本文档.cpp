#include<bits/stdc++.h>
using namespace std;
inline int read()
{
    int x = 0, f = 1; char ch = getchar();
    while (ch < '0' || ch>'9') { if (ch == '-') f = -1; ch = getchar(); }
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - 48; ch = getchar(); }
    return x * f;
}
inline void write(long long x)
{
    static char buf[30];
    int p = 0;
    if (x < 0) { putchar('-'); x = -x; }
    if (x == 0) buf[p++] = '0';
    while (x) { buf[p++] = x % 10 + 48; x /= 10; }
    while (p--) putchar(buf[p]);
}
int main()
{
    int T = read();
    while (T--)
    {
        int n = read();
        long long q = 1;
        int ans = -1;
        for(int i = 1;i <= n;i++)
        {
            long long a = read(), b = read();
            if (ans != -1) continue;
            if (q <= a) ans = 0;
            q = q - a + b;
        }
        if (ans == -1) write(n + 1);
        else write(0);
        putchar('\n');
    }
    return 0;
}
