#include<bits/stdc++.h>
#define int unsigned long long
using namespace std;
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

const int N=5e6+10,INF=1e9+10;
struct aqtw{ 
	int tot=0;
	struct redstone{
		int l=-1,r=-1;
		int lc=-1,rc=-1;
		int wh;
		int addz=0;
	} wcj[N];
	void fixtree(int u){
		if(wcj[u].l==-1||wcj[u].r==-1) return;
        int lc=wcj[u].lc, rc=wcj[u].rc;//记录左右儿子的指针。由于动态开点，原来 u<<1,u<<1|1 的用法不再生效
        wcj[u].wh = wcj[lc].wh + wcj[rc].wh;
    }

	void banzai(int p){
		if(wcj[p].l==-1||wcj[p].r==-1) return;
		int lc=wcj[p].lc, rc=wcj[p].rc;
		int lenL = wcj[lc].r - wcj[lc].l + 1;
		int lenR = wcj[rc].r - wcj[rc].l + 1;
		if(wcj[p].addz != 0){
			wcj[lc].wh += wcj[p].addz * lenL;
			wcj[rc].wh += wcj[p].addz * lenR;
			wcj[lc].addz += wcj[p].addz;
			wcj[rc].addz += wcj[p].addz;
			wcj[p].addz = 0;
		}
	}

	void init(){//初始化第一个节点
	    tot = 0;
	    wcj[1].l = wcj[1].r = 0;
	    wcj[1].wh = wcj[1].addz = 0;
	    wcj[1].lc = wcj[1].rc = -1;
	}

	int build(int l,int r,int x){//加入节点
		++tot;
		wcj[tot].l = l;
		wcj[tot].r = r;
		wcj[tot].wh=x;
		wcj[tot].addz = 0;
		wcj[tot].lc = wcj[tot].rc = -1;
		return tot;
	}

	void plustree(int u,int l,int r,int obl,int obr,int k){
		if(wcj[u].l==-1||wcj[u].r==-1) return;
		if(obl<=l && r<=obr){
            wcj[u].wh += k * (r-l+1);
            wcj[u].addz += k;
            return;
        }
        int mid=(l+r)>>1;
        int cns=0;
        if(wcj[u].lc==-1){
        	cns=(mid*(mid+1)/2)-(l*(l-1)/2);
			build(l,mid,cns);
			wcj[u].lc=tot;
		}
		if(wcj[u].rc==-1){
			cns=(r*(r+1)/2)-(mid*(mid+1)/2);
			build(mid+1,r,cns);
			wcj[u].rc=tot;
		}
        banzai(u);
        if(obl<=mid) plustree(wcj[u].lc,l,mid,obl,obr,k);
        if(obr>mid) plustree(wcj[u].rc,mid+1,r,obl,obr,k);
        fixtree(u);	
	}

	int findxigema(int u,int l,int r,int obl,int obr){
		if(wcj[u].l==-1||wcj[u].r==-1) return 0;
		if(obl<=l && r<=obr) return wcj[u].wh;
		int mid=(l+r)>>1;
		int cns=0;
		if(wcj[u].lc==-1){
		cns=(mid*(mid+1)/2)-(l*(l-1)/2);
			build(l,mid,cns);
			wcj[u].lc=tot;
		}
		if(wcj[u].rc==-1){
			cns=(r*(r+1)/2)-(mid*(mid+1)/2);
			build(mid+1,r,cns);
			wcj[u].rc=tot;
		}
		banzai(u);
		int res=0;
		if(obl<=mid) res += findxigema(wcj[u].lc,l,mid,obl,obr);
		if(obr>mid) res += findxigema(wcj[u].rc,mid+1,r,obl,obr);
		return res;
	}
} lzx;

signed main(){
	int n,m;
	n=read(); m=read(); 
	lzx.init();
	lzx.build(1,INF,(INF*(INF+1)/2));
	while(m--){
		int c,x,y,k;
		c=read();
		if(c==1){
			x=read(); y=read(); k=read();
			lzx.plustree(1,1,INF,x,y,k);
		}else{
			x=read(); y=read();
			int ans = lzx.findxigema(1,1,INF,x,y);
			write(ans);
			putchar('\n');
		}
	}
    return 0; 
}
