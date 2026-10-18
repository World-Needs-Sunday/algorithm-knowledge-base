#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=1e6+10;
//int cnt=0;
//int a[N];
struct AC{
	int c[N][26],val[N],fail[N],ans[N],ind[N],rev[N],cxc[N];
	int rot=0,cnt=0;
	int idx(char a){
		return (int)(a-'a');
	}
	void insert(string s,int x){
		int nrt=rot,len=s.size();
		for(int i=0;i<len;i++){
			int id=idx(s[i]);
			if(!c[nrt][id]) c[nrt][id]=++cnt;
			nrt=c[nrt][id];
		}
		if(!val[nrt]) val[nrt]=x;
		rev[x]=val[nrt];
	}
	void build(){
		queue<int> q;
		for(int i=0;i<26;i++){
			if(c[rot][i]){
				fail[c[rot][i]]=rot;
				q.push(c[rot][i]);
				ind[rot]++;
			}
		}
		while(q.size()){
			int x=q.front();q.pop();
			for(int i=0;i<26;i++){
				if(c[x][i]){
					fail[c[x][i]]=c[fail[x]][i];
					q.push(c[x][i]);
					ind[fail[c[x][i]]]++;
				}else c[x][i]=c[fail[x]][i];
			}
		}
	}
	void query(string s){
		int nrt=rot,len=s.size();
		for(int i=0;i<len;i++){
			int id=idx(s[i]);
			nrt=c[nrt][id];
			ans[nrt]++;
		}
	}
	void topo(){
		queue<int>q;
		for(int i=1;i<=cnt;i++){
			if(ind[i]==0)q.push(i);
		}
		while(!q.empty()){
			int tmp=q.front();
			q.pop();
			if(val[tmp]) cxc[val[tmp]]=ans[tmp];
			int x=fail[tmp];
			ind[x]--,ans[x]+=ans[tmp];
			if(ind[x]==0) q.push(x);
		}
		return;
	}
} zmy;
inline int read();
inline void write(int x);
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
	int n;cin>>n;
	for(int i=1;i<=n;i++){
		string s;cin>>s;
		zmy.insert(s,i);
	}
	zmy.build();
	string s;cin>>s;
	zmy.query(s);
	zmy.topo();
	for(int i=1;i<=n;i++){
		cout<<zmy.cxc[zmy.rev[i]]<<'\n';
	}
    return 0;
}
