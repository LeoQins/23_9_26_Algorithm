#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstring>
#include<vector>
#include<map>
using namespace std;

const int N = 1e5 + 25;

int son[N], siz[N], dep[N],fa[N];//ĳ�ڵ���ض��ӣ�ĳ�ڵ�������Ľڵ������ĳ�ڵ���ȣ�ĳ�ڵ������ϵĸ��ס�
int dfn[N],rnk[N],top[N];//ĳ�ڵ��DFS�򣬼��߶����еı�ţ�DFS������Ӧ�Ľڵ��ţ�ĳ�ڵ����������Ķ����ڵ㣨�����С�Ľڵ㣩��
int cnt; //DFS�������
int h[N], p[N],nxt[N];//ͷ�ڵ����飬h[o]ָ�ڵ�o�ĵ�һ���ߣ�p[j]��ʾ��j��Ŀ��ڵ㣬���Ҳ��h[o]���ӽڵ㣻nxt[j]��jͬ��h[0]�������ڽӱ�   //��ʼ���ݣ��ڽӱ�
int w[N];//ĳ�ߵ�Ȩֵ
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
void add_edge(int x, int y) {  // �ӱ�
    cur++;
    nxt[cur] = h[x];
    h[x] = cur;
    p[cur] = y;
    //w[cur] = z;
}

void dfs1(int o) {
    son[o] = -1;
    siz[o] = 1;
    for (int j = h[o]; j; j = nxt[j])       //jΪ��
        if (!dep[p[j]]) {//��ȵ�dep[p[j]]��ʾ��Ϊj�Ľڵ�p[j]��Ҳ��h[o]���ӽڵ㣩δ�����ʣ�Ӧ������
            dep[p[j]] = dep[o] + 1;
            fa[p[j]] = o;//��Ϊ�������κνڵ㣨�����ڵ㣩����Ⱦ�Ϊ1���� fa[p[j]] = o��Ȼ�޳�ͻ���������ƺ������ʡ�
            //o-->l,f,d  w[l]=w;
            dfs1(p[j]);
            siz[o] += siz[p[j]];
            if (son[o] == -1 || siz[p[j]] > siz[son[o]]) son[o] = p[j];
        }
}




void dfs2(int o, int t) {

    top[o] = t;     //�ڵ� o ���ڵ�������ͷ�ڵ�Ϊ t
    cnt++;          //ȫ��DFS����cnt++
    dfn[o] = cnt;//ӳ�� ʵ�ʽڵ�-->DFS����
    rnk[cnt] = o;//ӳ�� DFS����-->ʵ�ʽڵ�
    if (son[o] == -1) return;
    dfs2(son[o], t);  // ���ȶ��ض��ӽ��� DFS�����Ա�֤ͬһ�������ϵĵ� DFS ��������
    for (int j = h[o]; j; j = nxt[j])//����ȫ��
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

//�����߶�������
int sum[N << 2], lazy[N << 2], a[N];
int n, m;
void push_up(int root, int rt) {
    //sum[root] = sum[rt] + sum[rt + 1]; //�ܺ�
   // sum[root] = min(sum[rt], sum[rt + 1]);//��Ϊ��Сֵ,����sum[]��ʾ������Сֵ
    sum[root] = max(sum[rt], sum[rt + 1]);//��Ϊ���ֵ,����sum[]��ʾ�������ֵ
}
void push_down(int root, int rt, int len) {
    lazy[rt] += lazy[root]; lazy[rt + 1] += lazy[root]; //
    sum[rt] += lazy[root] * (len - len / 2); sum[rt + 1] += lazy[root] * (len / 2); //
    lazy[root] = 0;
}
void build(int root, int l, int r) {
    if (l == r) {
        //sum[root] = a[l]; return; // �ڵ��ֵ������
        sum[root] = posw[ rnk[l] ] [  fa[ rnk[l] ] ];//�ڵ��ֵ��Ӧ�븸��Ȩ  //�ѿ�����������
        
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
    dfs1(1);//Ĭ�ϴӽڵ�1��ʼ
    dfs2(1,1);
    posw[rnk[1]][0] = 0;//DFS��Ϊ1�Ľڵ�û����ȣ���ȨΪ0
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

void add_edge(int x, int y) {  // �ӱ�
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
h[o]=cur;��������


so

*/