#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int S = 1 << 20;
    int idx = 0, size = 0;
    char buf[S];
    inline char getch() {
        if (idx >= size) {
            size = (int)fread(buf, 1, S, stdin);
            idx = 0;
            if (size == 0) return EOF;
        }
        return buf[idx++];
    }
    template <class T>
    bool readInt(T &out) {
        char c = getch();
        if (c == EOF) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = getch();
            if (c == EOF) return false;
        }
        T sign = 1, x = 0;
        if (c == '-') {
            sign = -1;
            c = getch();
        }
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = getch();
        }
        out = x * sign;
        return true;
    }
};

struct Edge {
    int to;
    int next;
    int id;
};

int main() {
    FastScanner fs;
    int n, m;
    if (!fs.readInt(n)) return 0;
    fs.readInt(m);

    vector<int> head(n + 1, -1);
    vector<Edge> edges;
    edges.reserve(2LL * m + 5);
    vector<int> eu(m + 1), ev(m + 1);

    auto add_edge = [&](int u, int v, int id) {
        edges.push_back({v, head[u], id});
        head[u] = (int)edges.size() - 1;
    };

    for (int i = 1; i <= m; ++i) {
        int x, y;
        fs.readInt(x);
        fs.readInt(y);
        eu[i] = x;
        ev[i] = y;
        add_edge(x, y, i);
        add_edge(y, x, i);
    }

    vector<int> dfn(n + 1, 0), low(n + 1, 0);
    vector<int> estack;
    estack.reserve(m);
    vector<char> bad(n + 1, 0);
    vector<int> mark(n + 1, 0), pos(n + 1, -1);
    int timer = 0, seen = 0;

    function<void(int,int)> dfs = [&](int u, int peid) {
        dfn[u] = low[u] = ++timer;
        for (int ei = head[u]; ei != -1; ei = edges[ei].next) {
            int v = edges[ei].to;
            int id = edges[ei].id;
            if (!dfn[v]) {
                estack.push_back(id);
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    ++seen;
                    vector<int> nodes;
                    vector<pair<int, int>> comp;
                    while (true) {
                        int e = estack.back();
                        estack.pop_back();
                        comp.push_back({eu[e], ev[e]});
                        if (mark[eu[e]] != seen) {
                            mark[eu[e]] = seen;
                            pos[eu[e]] = (int)nodes.size();
                            nodes.push_back(eu[e]);
                        }
                        if (mark[ev[e]] != seen) {
                            mark[ev[e]] = seen;
                            pos[ev[e]] = (int)nodes.size();
                            nodes.push_back(ev[e]);
                        }
                        if (e == id) break;
                    }
                    int k = (int)nodes.size();
                    vector<vector<int>> g(k);
                    for (auto [a, b] : comp) {
                        int ia = pos[a], ib = pos[b];
                        if (ia == ib) continue;
                        g[ia].push_back(ib);
                        g[ib].push_back(ia);
                    }
                    vector<int> col(k, -1);
                    bool bip = true;
                    for (int i = 0; i < k && bip; ++i) {
                        if (col[i] != -1) continue;
                        queue<int> q;
                        col[i] = 0;
                        q.push(i);
                        while (!q.empty() && bip) {
                            int x = q.front();
                            q.pop();
                            for (int y : g[x]) {
                                if (col[y] == -1) {
                                    col[y] = col[x] ^ 1;
                                    q.push(y);
                                } else if (col[y] == col[x]) {
                                    bip = false;
                                    break;
                                }
                            }
                        }
                    }
                    if (!bip) {
                        for (int vtx : nodes) bad[vtx] = 1;
                    }
                }
            } else if (id != peid && dfn[v] < dfn[u]) {
                estack.push_back(id);
                low[u] = min(low[u], dfn[v]);
            }
        }
    };

    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) dfs(i, 0);
    }

    long long ans = 0;
    for (int i = 1; i <= n; ++i) if (!bad[i]) ++ans;
    cout << ans << '\n';
    return 0;
}
