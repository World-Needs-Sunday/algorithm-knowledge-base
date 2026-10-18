struct Node {
    int l, r;
    mutable long long v;

    bool operator<(const Node &other) const {
        return l < other.l;
    }
};

set<Node> odt;

// 最简单的单点装入初始化
for (int i = 1; i <= n; ++i) {
    odt.insert({i, i, a[i]});
}

auto split(int pos) {
    if (pos > n) return odt.end();

    // 找到第一个左端点 >= pos 的区间
    auto it = odt.lower_bound({pos, -1, 0});
    if (it != odt.end() && it->l == pos) {
        return it;
    }

    // 真正的目标区间在 it 的前一个
    --it;
    int l = it->l, r = it->r;
    long long v = it->v;

    // 先删除原区间，再插入切分后的两个区间
    odt.erase(it);
    odt.insert({l, pos - 1, v});
    return odt.insert({pos, r, v}).first;
}

void assign(int l, int r, long long x) {
    auto itr = split(r + 1);
    auto itl = split(l);

    // 删除 [itl, itr) 内的所有旧区间
    odt.erase(itl, itr);

    // 插入合并后的大区间
    odt.insert({l, r, x});
}

void add(int l, int r, long long val) {
    auto itr = split(r + 1);
    auto itl = split(l);

    for (auto it = itl; it != itr; ++it) {
        it->v += val; // 由于使用了 mutable，可以直接修改
    }
}

long long query_kth(int l, int r, int k) {
    auto itr = split(r + 1);
    auto itl = split(l);

    vector<pair<long long, int>> elements;
    for (auto it = itl; it != itr; ++it) {
        elements.push_back({it->v, it->r - it->l + 1});
    }

    sort(elements.begin(), elements.end());

    for (auto &[val, cnt] : elements) {
        k -= cnt;
        if (k <= 0) return val;
    }
    return -1;
}

long long query_power_sum(int l, int r, long long ex, long long mod) {
    auto itr = split(r + 1);
    auto itl = split(l);

    long long sum = 0;
    for (auto it = itl; it != itr; ++it) {
        long long len = it->r - it->l + 1;
        sum = (sum + len * qpow(it->v % mod, ex, mod)) % mod;
    }
    return sum;
}
