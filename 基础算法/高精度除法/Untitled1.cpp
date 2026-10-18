#include <bits/stdc++.h>
using namespace std;

// 高精度除以低精度：返回 pair<商, 余数>
pair<string,int> divmod(string a,int b){
    string q;          // 商
    long long r=0;     // 当前余数
    bool lead=true;    // 是否仍处于前导零阶段
    for(char c:a){
        r=r*10+(c-'0');   // 余数左移一位，拼上当前数字位
        int d=r/b;        // 试商：当前余数能容纳多少个 b
        r%=b;             // 更新余数
        if(!lead||d!=0){  // 跳过前导零：已有非零商或当前位商非零才记录
            q+=d+'0';
            lead=false;
        }
    }
    if(q.empty()) q="0";  // 商为 0 时也要保留一个 "0"
    return {q,(int)r};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string a;
    int b;
    cin >> a >> b;
    pair<string,int> p = divmod(a, b);
    cout << p.first << '\n';   // 商
    cout << p.second << '\n';  // 余数
    return 0;
}
