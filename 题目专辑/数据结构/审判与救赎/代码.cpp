#include<bits/stdc++.h>
using namespace std;
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define mid ((l+r)>>1)
#define lc goc[u].ls
#define rc goc[u].rs

typedef pair<int,int> PII;
typedef long long ll;
const int N=5e5+10;
const int INF=1e9+10;
const int mod=998244353;

int a[N], h1[N], hf0[N], hmod[N];

inline int ksm(int x,int y,int p){
    int res=1%p;
    x%=p;
    while(y){
        if(y&1) res=(ll)res*x%p;
        x=(ll)x*x%p;
        y>>=1;
    }
    return res;
}

unordered_map<int,int> ic, fc, ifc;

struct siryessir{
    struct note{
        int ls=0, rs=0;
        int wh=1, iwh=1;
        int cnt=0;
        int osp=1, iosp=1;
    };
    vector<note> goc;
    int tot=0;
    int rt[N];

    inline int getinv(int x){
        x %= mod;
        if(x==0) return 0;
        auto it = ic.find(x);
        if(it != ic.end()){
            int res = it->second;
            return res;
        }
        int res = ksm(x, mod-2, mod);
        ic[x] = res;
        return res;
    }

    inline int getf(int x){
        x %= mod;
        auto it = fc.find(x);
        if(it != fc.end()){
            int res = it->second;
            return res;
        }
        int invx = getinv(x);
        int res = (ll)(x+1) * invx % mod;
        fc[x] = res;
        return res;
    }

    inline int getif(int x){
        x %= mod;
        auto it = ifc.find(x);
        if(it != ifc.end()){
            int res = it->second;
            return res;
        }
        int invx1 = getinv((x+1) % mod);
        int res = (ll)x * invx1 % mod;
        ifc[x] = res;
        return res;
    }

    siryessir(){
        goc.clear();
        tot=0;
        goc.push_back({0,0,1,1,0,1,1});
    }

    int zxpro(int u,int l,int r,int k,int v){
        if(k==0) return u;
        goc.push_back(goc[u]);
        u=++tot;

        int kmod = k % mod;
        goc[u].wh = (ll)goc[u].wh * kmod % mod;
        goc[u].iwh = (ll)goc[u].iwh * getinv(kmod) % mod;

        if((k & 1) == 0){
            goc[u].cnt += v;
            int inv_k = getinv(kmod);
            int inv_k1 = getinv((kmod + 1) % mod);
            int f = (ll)(kmod + 1) * inv_k % mod;
            int vf = (ll)kmod * inv_k1 % mod;
            goc[u].osp = (ll)goc[u].osp * f % mod;
            goc[u].iosp = (ll)goc[u].iosp * vf % mod;
        }

        if(l==r) return u;
        if(k <= mid) goc[u].ls = zxpro(goc[u].ls, l, mid, k, v);
        else goc[u].rs = zxpro(goc[u].rs, mid+1, r, k, v);

        int lu = goc[u].ls, ru = goc[u].rs;
        goc[u].wh = (ll)goc[lu].wh * goc[ru].wh % mod;
        goc[u].iwh = (ll)goc[lu].iwh * goc[ru].iwh % mod;
        goc[u].cnt = goc[lu].cnt + goc[ru].cnt;
        goc[u].osp = (ll)goc[lu].osp * goc[ru].osp % mod;
        goc[u].iosp = (ll)goc[lu].iosp * goc[ru].iosp % mod;
        return u;
    }

    int aliez(int p1,int p2,int l,int r,int k){
        if(k <= 0) return 1;
        int cntz = goc[p2].cnt - goc[p1].cnt;
        if(k > cntz) k = cntz;
        if(k == 0) return 1;
        if(l == r){
            return ksm(getf(l), k, mod);
        }
        int lu1 = goc[p1].ls, ru1 = goc[p1].rs;
        int lu2 = goc[p2].ls, ru2 = goc[p2].rs;
        int now = goc[lu2].cnt - goc[lu1].cnt;
        if(k <= now){
            return aliez(lu1, lu2, l, mid, k);
        } else {
            int cntl = (ll)goc[lu2].osp * goc[lu1].iosp % mod;
            int cntr = aliez(ru1, ru2, mid+1, r, k - now);
            return (ll)cntl * cntr % mod;
        }
    }
} hjt;

inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch< '0'||ch>'9'){
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9'){
        x=(x<<1)+(x<<3)+(ch^48);
        ch=getchar();
    }
    return x*f;
}
inline void write(int x){
    if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9) write(x/10);
    putchar(x%10+'0');
}

int main(){
    int n, m;
    n = read(); m = read();
    for(int i=1;i<=n;i++){
        a[i] = read();
        h1[i] = h1[i-1] + (a[i] == 1);
        hf0[i] = hf0[i-1] + (a[i] != 0);
        hmod[i] = hmod[i-1] + (a[i] == mod);
        if(a[i] == mod){
            hjt.rt[i] = hjt.rt[i-1];
        } else {
            hjt.rt[i] = hjt.zxpro(hjt.rt[i-1], -INF, INF, a[i], 1);
        }
    }
    while(m--){
        int l, r;
        l = read(); r = read();
        if(hf0[r] - hf0[l-1] == 0){
            write(0); putchar('\n');
            continue;
        }
        if(hmod[r] - hmod[l-1] > 0){
            write(0); putchar('\n');
            continue;
        }
        int res = (ll)hjt.goc[hjt.rt[r]].wh * hjt.goc[hjt.rt[l-1]].iwh % mod;
        int cnt1 = h1[r] - h1[l-1];
        int cntj = hjt.goc[hjt.rt[r]].cnt - hjt.goc[hjt.rt[l-1]].cnt;
        int k = min(cnt1, cntj);
        if(k == 0){
            write(res); putchar('\n');
            continue;
        }
        int ans = hjt.aliez(hjt.rt[l-1], hjt.rt[r], -INF, INF, k);
        write((ll)res * ans % mod); putchar('\n');
    }
    return 0;
}
