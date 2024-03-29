// codigo de: https://github.com/mpfeifer1/Kattis/blob/master/dutyscheduler.cpp

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

const ll MAXN = 60 + 31 + 2 + 14; // Arbitrary
const ll INF = (ll)1000000000;

struct edge {
    ll a, b, cap, flow;
};

ll n, s, t, d[MAXN], ptr[MAXN], q[MAXN];
vector<edge> e;
vector<ll> g[MAXN];

void addedge(ll a, ll b, ll cap) {
    edge e1 = { a, b, cap, 0 };
    edge e2 = { b, a, 0, 0 };
    g[a].push_back((ll) e.size());
    e.push_back(e1);
    g[b].push_back((ll) e.size());
    e.push_back(e2);
}

// implementacion del algoritmo de dinic para encontrar el flujo maximo

bool bfs() {
    ll qh=0, qt=0;
    q[qt++] = s;
    memset(d, -1, n * sizeof d[0]);
    d[s] = 0;
    while(qh < qt && d[t] == -1) {
        ll v = q[qh++];
        for(size_t i=0; i<g[v].size(); ++i) {
            ll id = g[v][i],
                to = e[id].b;
            if(d[to] == -1 && e[id].flow < e[id].cap) {
                q[qt++] = to;
                d[to] = d[v] + 1;
            }
        }
    }
    return d[t] != -1;
}

ll dfs(ll v, ll flow) {
    if(!flow)  return 0;
    if(v == t)  return flow;
    for(; ptr[v]<(ll)g[v].size(); ++ptr[v]) {
        ll id = g[v][ptr[v]];
        ll to = e[id].b;
        if(d[to] != d[v] + 1)  continue;
        ll pushed = dfs(to, min (flow, e[id].cap - e[id].flow));
        if(pushed) {
            e[id].flow += pushed;
            e[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

ll dinic() {
    ll flow = 0;
    for(;;) {
        if(!bfs())  break;
        memset(ptr, 0, n * sizeof ptr[0]);
        while(ll pushed = dfs(s,INF)) {
            flow += pushed;
        }
    }
    return flow;
}

// Reset dinic
void reset() {
    e.clear();
    for(auto& i : g) {
        i.clear();
    }
    for(auto& i : d) i = 0;
    for(auto& i : ptr) i = 0;
    for(auto& i : q) i = 0;
}

bool works(int ra_count, int maxdays, int month, map<string,vector<int>>& ra, vector<string>& num_to_ra, map<string,int>& ra_to_num) {
    
    reset();

    // numero total de nodos
    ::n = ra_count + month + 2;

    // establece el source y sink de la red de flujo
    s = ::n-1;
    t = ::n-2;

    // nodos RA 0 hasta ra_count
    for(int i = 0; i < ra_count; i++) {
        // añade a la red con peso igual a max_days
        addedge(s,i,maxdays);
    }

    // nodos dias de ra_count hasta ra_count+month
    for(int i = ra_count; i < ra_count + month; i++) {
        // añade a la red con peso igual a 2, q es el maximo por dia
        addedge(i,t,2);
    }

    // añade desde RA hasta los dias asignados con peso 1
    for(auto i : ra) {
        int n1 = ra_to_num[i.first];
        for(auto j : i.second) {
            addedge(n1, ra_count+j-1, 1);
        }
    }

    // si el flujo maximo es igual a 2 veces la cantidad de RA
    // entonces se pueden asignar
    int flow = dinic();
    if(flow == month*2) {
        cout << maxdays << endl;

        vector<vector<string>> ans(month);
        for(auto i : e) {
            if(i.a < ra_count && i.flow == 1) {
                ans[i.b - ra_count].push_back(num_to_ra[i.a]);
            }
        }

        for(int i = 0; i < month; i++) {
            cout << "Day " << i+1 << ": ";
            for(auto j : ans[i]) {
                cout << j << " ";
            }
            cout << endl;
        }

        return true;
    }

    return false;

}

int main() {
    int m, n;
    cin >> m >> n;

#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
#endif

    map<string, vector<int>> ra;
    map<string,int> ra_to_num;
    vector<string> num_to_ra;
    for(int i = 0; i < m; i++) {
        string s;
        cin >> s;
        int t;
        cin >> t;
        vector<int> v(t);
        for(auto& i : v) {
            cin >> i;
        }
        ra[s] = v;
        num_to_ra.push_back(s);
        ra_to_num[s] = i;
    }

    // prueba maximo numero de dias desde i hasta n.
    for(int i = 1; i <= n; i++) {
        if(works(m,i,n,ra,num_to_ra,ra_to_num)) {
            break;
        }
    }
}