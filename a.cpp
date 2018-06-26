#include <iostream>
#include <vector>

using namespace std;

struct edge {
    int me;
    int to;
    int cur_flow;
    int max_flow;
    int cost;
    int back;
};

int n, m, k, s = 0;
bool haveput = false;
vector<bool> used;
vector<edge> edges;
vector<vector<int>> g;
vector<int> d, parent, potencials, pp;

void ford_bellman() {
    d[s] = 0;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < edges.size(); ++j) {
            if (d[edges[j].me] < 1e9 && edges[j].max_flow > 0) {
                d[edges[j].to] = min(d[edges[j].to], d[edges[j].me] + edges[j].cost);
            }
        }
    }
}

void dekstr() {
    used.clear();
    used.resize(n, false);
    d.clear();
    d.assign(n, 1e9);
    parent.clear();
    parent.resize(n, 0);
    haveput = false;

    d[s] = 0;
    for (int i = 0; i != n; ++i) {
        int v = -1;
        for (int j = 0; j != n; ++j) {
            if (!used[j] && (v == -1 || d[j] < d[v])) {
                v = j;
            }
        }

        if (d[v] == 1e9)
            break;
        used[v] = true;
        
        for (int j = 0; j != g[v].size(); ++j) {
            int to = edges[g[v][j]].to,
                    len = edges[g[v][j]].cost + potencials[v] - potencials[to];
            if (d[v] + len < d[to] && edges[g[v][j]].cur_flow < edges[g[v][j]].max_flow) {
                haveput = true;
                d[to] = d[v] + len;
                parent[to] = g[v][j];
            }
        }
    }
}

void put() {
    pp.clear();
    for (int i =0; i != n; ++i) {
        //cout << i << ' ' << edges[parent[i]].me << " " << parent[i] << endl;
    }
    if (haveput)
        for (int i = n - 1; i != 0; i = edges[parent[i]].me) {
            //cout << "! ! !   " << i << ' ' << edges[parent[i]].me << " " << parent[i] << endl;
            pp.push_back(parent[i]);
            if (edges[parent[i]].me == i) {
                return;
            }
        }
}

void update_flow() {
    for (int i = pp.size() - 1; i >= 0; --i) {
        ++edges[pp[i]].cur_flow;
        --edges[edges[pp[i]].back].cur_flow;
    }
    pp.resize(0);
}

vector<int> dd;
int dfs(vector<int> & path, vector<vector<int>> & f, int v) {
    if (v == n - 1) {
        return 0;
    }

    for (int i = 0; i != f[v].size(); ++i) {
        if (edges[f[v][i]].cur_flow == 1) {
            edges[f[v][i]].cur_flow = 0;
            path.push_back(f[v][i] / 4 + 1);
            dd.push_back(f[v][i]);
            return dfs(path, f, edges[f[v][i]].to) + edges[f[v][i]].cost;
        }
    }
}


int main() {
    cin >> n >> m >> k;
    potencials.resize(n, 1e9);
    parent.resize(n, 0);
    d.resize(n, 1e9);
    g.resize(n + 1, vector<int> ());
    used.resize(n + 1, false);
    int a, b, c;
    for (int i = 0; i != m; ++i) {
        cin >> a >> b >> c;
        int r = (int)edges.size();
        edges.push_back({a-1, b-1, 0, 1, c, r + 1});
        edges.push_back({b-1, a-1, 0, 0, -c, r});
        g[a-1].push_back(r);
        g[b-1].push_back(r + 1);

        r = (int)edges.size();
        edges.push_back({a-1, b-1, 0, 0, -c, r + 1});
        edges.push_back({b-1, a-1, 0, 1, c, r});
        g[a-1].push_back(r);
        g[b-1].push_back(r + 1);
    }

    ford_bellman();
    for (int i = 0; i != n; ++i) {
        potencials[i] = d[i];
    }

    for (int q = 0; q != k; ++q) {
        dekstr();
        put();
        if (haveput)
            update_flow();
        dekstr();
        for (int i = 0; i != n; ++i)
            potencials[i] = min(1000000000, d[i] + potencials[i]);
    }
    int sum_flow = 0;
    for (int i = 0; i != g[0].size(); ++i) {
        sum_flow += edges[g[0][i]].cur_flow;
    }

    if (sum_flow < k) {
        cout << -1;
        return 0;
    }

    vector<vector<int>> answer(k, vector<int>());
    vector<vector<int>> f = g;
    int ans = 0;
    for (int i = 0; i != k; ++i) {
        ans += dfs(answer[i], f, 0);
        g = f;
    }

    cout.precision(20);
    cout << (double) ans / k << endl;
    for (int i = 0; i != k; ++i) {
        cout << answer[i].size() << ' ';
        for (int j = 0; j != answer[i].size(); ++j) {
            cout << answer[i][j] << ' ';
        }
        cout << endl;
    }

    return 0;
}
