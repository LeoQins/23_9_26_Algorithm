#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstring>
#include<vector>
#include<map>
using namespace std;

const int N = 1e5 + 25;

int son[N], siz[N], dep[N],fa[N];//某节点的重儿子；某节点的子树的节点个数；某节点深度，某节点在树上的父亲。
int dfn[N],rnk[N],top[N];//某节点的DFS序，即线段树中的编号；DFS序所对应的节点编号；某节点所在重链的顶部节点（深度最小的节点）。
int cnt; //DFS序计数器
int h[N], p[N],nxt[N];//头节点数组，h[o]指节点o的第一条边；p[j]表示边j的目标节点，因此也是h[o]的子节点；nxt[j]和j同属h[0]，都是邻接边   //初始数据，邻接表
int w[N];//某边的权值
int cur;//=0

map<int,map<int, int> >posw;

void init() {
    for (int i = 1; i <= N; i++) {
        son[i] = 0;
        siz[i] = 0;
        dep[i] = 0;
        fa[i] = 0;
        dfn[i] = 0;
        rnk[i] = 0;
        top[i] = 0;
        cnt = 0;
        h[i] = 0;
        p[i] = 0;
        nxt[i] = 0;
        w[i] = 0;
    }
    dep[1] = 1;
}
void add_edge(int x, int y) {  // 加边
    cur++;
    nxt[cur] = h[x];
    h[x] = cur;
    p[cur] = y;
    //w[cur] = z;
}

void dfs1(int o) {
    son[o] = -1;
    siz[o] = 1;
    for (int j = h[o]; j; j = nxt[j])       //j为边
        if (!dep[p[j]]) {//深度的dep[p[j]]表示边为j的节点p[j]（也是h[o]的子节点）未曾访问，应当处理
            dep[p[j]] = dep[o] + 1;
            fa[p[j]] = o;//因为是树，任何节点（除根节点）的入度均为1，故 fa[p[j]] = o必然无冲突断链；类似函数性质。
            //o-->l,f,d  w[l]=w;
            dfs1(p[j]);
            siz[o] += siz[p[j]];
            if (son[o] == -1 || siz[p[j]] > siz[son[o]]) son[o] = p[j];
        }
}




void dfs2(int o, int t) {

    top[o] = t;     //节点 o 所在的重链的头节点为 t
    cnt++;          //全局DFS序编号cnt++
    dfn[o] = cnt;//映射 实际节点-->DFS序编号
    rnk[cnt] = o;//映射 DFS序编号-->实际节点
    if (son[o] == -1) return;
    dfs2(son[o], t);  // 优先对重儿子进行 DFS，可以保证同一条重链上的点 DFS 序连续。
    for (int j = h[o]; j; j = nxt[j])//遍历全部
        if (p[j] != son[o] && p[j] != fa[o]) dfs2(p[j], p[j]);

}



int lca(int u, int v) {

    while (top[u] != top[v]) {
        if (dep[top[u]] > dep[top[v]])
            u = fa[top[u]];
        else
            v = fa[top[v]];
    }
    return dep[u] > dep[v] ? v : u;

}

//区间线段树部分
int sum[N << 2], lazy[N << 2], a[N];
int n, m;
void push_up(int root, int rt) {
    //sum[root] = sum[rt] + sum[rt + 1]; //总和
   // sum[root] = min(sum[rt], sum[rt + 1]);//改为最小值,次数sum[]表示区间最小值
    sum[root] = max(sum[rt], sum[rt + 1]);//改为最大值,次数sum[]表示区间最大值
}
void push_down(int root, int rt, int len) {
    lazy[rt] += lazy[root]; lazy[rt + 1] += lazy[root]; //
    sum[rt] += lazy[root] * (len - len / 2); sum[rt + 1] += lazy[root] * (len / 2); //
    lazy[root] = 0;
}
void build(int root, int l, int r) {
    if (l == r) {
        //sum[root] = a[l]; return; // 节点的值即它的
        sum[root] = posw[ rnk[l] ] [  fa[ rnk[l] ] ];//节点的值对应与父边权  //已考虑无入度情况
        
    }
    int rt = root << 1, mid = (l + r) >> 1;
    build(rt, l, mid); build(rt + 1, mid + 1, r);
    push_up(root, rt);
}
void update(int root, int l, int r, int ul, int ur, int val) {
    int rt = root << 1, mid = (l + r) >> 1, len = r - l + 1;
    if (l > ur || r < ul) return;
    else if (l >= ur && r <= ul) {
        sum[root] += len * val; //
        if (r > l) lazy[root] += val;  //
    }
    else {
        push_down(root, rt, len);
        update(rt, l, mid, ul, ur, val);
        update(rt + 1, mid + 1, r, ul, ur, val);
        push_up(root, rt);
    }
}
int query(int root, int l, int r, int ql, int qr) {
    int ans = 0, rt = root << 1, mid = (l + r) >> 1, len = r - l + 1;
    if (qr < l || r < ql) return ans;
    else if (ql <= l && r <= qr) return sum[root];
    else {
        push_down(root, rt, len);
        ans += query(rt, l, mid, ql, qr);//
        ans += query(rt + 1, mid + 1, r, ql, qr);//
        return ans;
    }
}




int solve() {
    int n; cin >> n;
    int a, b, c;
    for (int i = 1; i <= n - 1; i++) {
        cin >> a >> b>>c;
        add_edge(a,b);
        add_edge(b, a);
        posw[a][b] = c; posw[b][a] = c;
        
    }
    dfs1(1);//默认从节点1开始
    dfs2(1,1);
    posw[rnk[1]][0] = 0;//DFS序为1的节点没有入度，边权为0
    string s; cin >> s;
    switch (s[0]) {
        case 'Q': {

            break;
        }
        case 'C': {
            int i, v; cin >> i >> v;
            update(1,1,n,)
            break;
        }
        case 'N': {
            
            break;
        }
        case 'D': {
            
            return 0;
        }
    }


    return 0;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int t; cin >> t;
    while (t--) {
        init();
        solve();
    }


}

/*

void add_edge(int x, int y) {  // 加边
  cur++;
  nxt[cur] = h[x];
  h[x] = cur;
  p[cur] = y;
}


cur++
h[o]==xx;

add a new one

o--v;
nxt[cur]=h[o];
h[o]=cur;增加链表


so

*/