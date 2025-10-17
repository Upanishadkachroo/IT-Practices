#include <bits/stdc++.h>
using namespace std;

// ----------------- Graph structure -----------------
struct Graph {
    int V;
    vector<vector<int>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }
};

// ----------------- Betweenness Centrality (Brandes) -----------------
vector<double> betweenness_centrality(Graph &g) {
    int V = g.V;
    vector<double> bc(V, 0.0);

    for(int s=0; s<V; s++){
        stack<int> S;
        vector<vector<int>> P(V);
        vector<int> sigma(V, 0);
        vector<int> d(V, -1);

        sigma[s] = 1;
        d[s] = 0;
        queue<int> Q;
        Q.push(s);

        while(!Q.empty()) {
            int v = Q.front(); Q.pop();
            S.push(v);
            for(int w : g.adj[v]){
                if(d[w]<0){
                    Q.push(w);
                    d[w] = d[v]+1;
                }
                if(d[w] == d[v]+1){
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }

        vector<double> delta(V,0);
        while(!S.empty()){
            int w = S.top(); S.pop();
            for(int v : P[w]){
                delta[v] += ((double)sigma[v]/sigma[w])*(1+delta[w]);
            }
            if(w != s) bc[w] += delta[w];
        }
    }
    return bc;
}

// ----------------- Simple Community Detection -----------------
vector<int> community_detection(Graph &g) {
    int V = g.V;
    vector<int> community(V);
    for(int i=0;i<V;i++) community[i] = i;

    for(int u=0; u<V; u++){
        for(int v : g.adj[u]){
            if(rand()%2) community[v] = community[u]; // random merge
        }
    }
    return community;
}

// ----------------- Save graph and centrality -----------------
void save_graph(Graph &g, vector<int> &community, string filename) {
    ofstream fout(filename);
    for(int u=0; u<g.V; u++){
        for(int v : g.adj[u]){
            if(u<v) 
                fout << u << " " << v << " " << community[u] << "\n";
        }
    }
    fout.close();
}

void save_centrality(vector<double> &bc, string filename) {
    ofstream fout(filename);
    for(int i=0;i<bc.size();i++) fout << i << " " << bc[i] << "\n";
    fout.close();
}

// ----------------- Main -----------------
int main() {
    srand(time(0));

    int V = 10; // number of nodes
    Graph g(V);

    // Random edges
    for(int i=0;i<V;i++){
        for(int j=i+1;j<V;j++){
            if(rand()%2) g.add_edge(i,j);
        }
    }

    // Compute centrality
    vector<double> bc = betweenness_centrality(g);
    save_centrality(bc, "output/centrality.txt");

    // Detect communities
    vector<int> comm = community_detection(g);
    save_graph(g, comm, "output/graph.txt");

    cout << "Computation done. Files saved in output/" << endl;
    return 0;
}
