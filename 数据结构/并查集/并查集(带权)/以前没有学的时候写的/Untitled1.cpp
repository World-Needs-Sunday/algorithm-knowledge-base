#include <bits/stdc++.h>
using namespace std;
/*蓝桥1156*/ 
long long N , M;
vector<long long> A;
vector<long long> k;
vector<long long> sz;
void init()
{
  A.resize(N + 1 , 0);
  for(long long i = 1;i <= N;i++) A[i] = i;
  k.resize(N + 1 , 0);
  sz.resize(N + 1 , 1);
}

long long find(long long idx)
{
  long long old = A[idx];
  if(A[idx] != idx) A[idx] = find(A[idx]);
  if(old != A[idx]) k[idx] += k[old];
  return A[idx];
}

bool unity(long long x,long long y)
{
  long long rx = find(x);
  long long ry = find(y);
  if(rx == ry) return false;
  k[rx] = sz[ry];
  sz[ry] += sz[rx];
  A[rx] = ry;
  return true;
}

int main()
{
  cin >> N >> M;
  init();
  for(long long i = 0;i < M;i++)
  {
    long long p , x , y;
    cin >> p >> x >> y;
    if(p == 1) unity(x,y);
    else
    {
      int rx = find(x);
      int ry = find(y);
      if(rx != ry) cout << -1 << endl;
      else cout << max(k[x],k[y]) - min(k[x],k[y]) - 1 << endl;
    }
  }
  return 0;
}
/*首先，最朴素的想法就是，每个人单独看成一个节点，如果执行并列操作时，就将x放到y后面（x节点的指针指向y）。进行搜索操作时，就分别计算出x与y到根节点的距离，然后进行相减即可。
如果熟悉并查集的话，就可以发现这就是不带搜索优化的并查集，但是在搜索的同时，要记录到根节点的距离。这个算法实现起来很简单，但时间复杂度是n的平方，在本题明显会超过时间。
那么就要进行思考，可不可以弄一个标记，记录每个节点到根节点的距离。并且可以支持并查集的优化查询操作。
对于节点x插入到y链后面时，x的节点统一都加上了y链的大小。我们可以凭借此为出发点。
初始创建一个标记，代表到根节点距离，因为刚开始的时候，大家都是自己的根节点，所以都为零。在执行并列操作的时候，x根节点指向y的根节点。即x所在链的所有根节点都变了，并且增加值为y链大小。因为并查集不支持从上往下进行查询，所以我们就可以在节点查询的同时，判断自己的根节点是否被改变，如果被改变，则将自己的标记加上自己旧根节点的标记。（因为原先的旧根节点标记为零，即这个节点的标记及就是距离这个旧根节点的位置。这个在更新之后，这两个相对距离依旧不会改变）然后我们会发现第一个根节点应该如何改变呢？那这个操作的话，就很显然适合在合列当中了，x根节点的距离原先为零，然后将其更改为y链的大小就好了。y链的大小也可以在合并时计算。初始都为一，然后合并的话就是x链的大小加y链的大小。

因为以上思路是找到自己所需的条件，然后缺啥补啥，所以会有点乱，现在来总结一下思路。
首先初始化，一个父节点fa[]初始化为自己，到根节点的距离k[]初始化为零，该键的大小sz[]初始化为1。
合并操作（1）x链的根节点设为y链大小。（2）y链的大小等于y链的大小加上x链的大小。（3）X根节点指向y的根节点。

查询操作（1）保存旧根节点（2）找到当前根节点（3）判断新旧根节点是否相同，如果相同则不管，如果不相同则加上旧根节点的k值。

搜索距离根节点长度操作，判断根节点是否是自己，如果是的话直接输出k。如果不是的话，加上自己的根节点的k值，进行输出。*/
