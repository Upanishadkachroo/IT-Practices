#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    double weight;
};

struct Graph {
    int V;
    vector<vector<pair<int,double>>> adj;
    vector<Edge> edges;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void add_edge(int u, int v, double w) {
        adj[u].push_back({v,w});
        adj[v].push_back({u,w}); // undirected
        edges.push_back({u,v,w});
    }

    void save_edges(string filename) {
        ofstream fout(filename);
        for(auto &e: edges)
            fout << e.u << " " << e.v << " " << e.weight << "\n";
        fout.close();
    }
};

// ----------------- Prim's Algorithm -----------------
vector<Edge> prim_mst(Graph &g) {
    int V = g.V;
    vector<bool> inMST(V,false);
    vector<double> key(V,1e9);
    vector<int> parent(V,-1);
    key[0] = 0;

    auto cmp = [](pair<double,int> a, pair<double,int> b){ return a.first > b.first; };
    priority_queue<pair<double,int>, vector<pair<double,int>>, decltype(cmp)> pq(cmp);
    pq.push({0,0});

    while(!pq.empty()) {
        int u = pq.top().second; pq.pop();
        if(inMST[u]) continue;
        inMST[u] = true;

        for(auto &p : g.adj[u]) {
            int v = p.first; double w = p.second;
            if(!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    vector<Edge> mst;
    for(int v=1; v<V; v++)
        mst.push_back({parent[v], v, key[v]});
    return mst;
}

// ----------------- Kruskal's Algorithm -----------------
struct DSU {
    vector<int> parent, rank;
    DSU(int n): parent(n), rank(n,0){ for(int i=0;i<n;i++) parent[i]=i; }
    int find(int x){ return parent[x]==x?x:parent[x]=find(parent[x]); }
    bool unite(int x,int y){
        int px=find(x), py=find(y);
        if(px==py) return false;
        if(rank[px]<rank[py]) swap(px,py);
        parent[py]=px;
        if(rank[px]==rank[py]) rank[px]++;
        return true;
    }
};

vector<Edge> kruskal_mst(Graph &g) {
    vector<Edge> mst;
    sort(g.edges.begin(), g.edges.end(), [](Edge a, Edge b){ return a.weight < b.weight; });
    DSU dsu(g.V);
    for(auto &e: g.edges)
        if(dsu.unite(e.u,e.v))
            mst.push_back(e);
    return mst;
}

// ----------------- Save MST -----------------
void save_mst(vector<Edge> &mst, string filename) {
    ofstream fout(filename);
    for(auto &e: mst)
        fout << e.u << " " << e.v << " " << e.weight << "\n";
    fout.close();
}

// ----------------- Main -----------------
int main() {
    srand(time(0));
    int V = 6;
    Graph g(V);

    // Random weighted edges
    for(int i=0;i<V;i++)
        for(int j=i+1;j<V;j++)
            if(rand()%2)
                g.add_edge(i,j, 1+rand()%10);

    g.save_edges("edges.txt");

    auto prim = prim_mst(g);
    save_mst(prim, "prim_mst.txt");

    auto kruskal = kruskal_mst(g);
    save_mst(kruskal, "kruskal_mst.txt");

    cout << "Graph and MSTs saved in current directory" << endl;
    return 0;
}
