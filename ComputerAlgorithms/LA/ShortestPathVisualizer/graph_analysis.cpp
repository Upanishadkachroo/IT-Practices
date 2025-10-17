#include <bits/stdc++.h>
using namespace std;

struct Edge { int to; double weight; };

struct Graph {
    int V;
    vector<vector<Edge>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void add_edge(int u, int v, double w) {
        adj[u].push_back({v,w});
        adj[v].push_back({u,w}); // undirected
    }
};

// ----------------- Dijkstra -----------------
vector<double> dijkstra(Graph &g, int src, vector<int> &parent) {
    int V = g.V;
    vector<double> dist(V, 1e9);
    parent.assign(V, -1);
    dist[src] = 0;

    using P = pair<double,int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, src});

    while(!pq.empty()) {
        auto [d,u] = pq.top(); pq.pop();
        if(d > dist[u]) continue;

        for(auto &e : g.adj[u]) {
            int v = e.to;
            double w = e.weight;
            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// ----------------- Save Graph & Shortest Paths -----------------
void save_graph(Graph &g, string filename) {
    ofstream fout(filename);
    for(int u=0; u<g.V; u++)
        for(auto &e : g.adj[u])
            if(u < e.to)
                fout << u << " " << e.to << " " << e.weight << "\n";
    fout.close();
}

void save_shortest_paths(int src, const vector<double> &dist, const vector<int> &parent, string filename) {
    ofstream fout(filename, ios::app);
    fout << "Source: " << src << "\n";
    for(int v=0; v<dist.size(); v++)
        fout << v << " " << dist[v] << " " << parent[v] << "\n";
    fout << "\n";
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

    save_graph(g, "edges.txt");

    for(int src=0; src<V; src++) {
        vector<int> parent;
        vector<double> dist = dijkstra(g, src, parent);
        save_shortest_paths(src, dist, parent, "shortest_paths.txt");
    }

    cout << "Graph and shortest paths saved in current directory" << endl;
    return 0;
}
