import matplotlib.pyplot as plt
import networkx as nx

# ----------------- Load Graph -----------------
G = nx.Graph()
with open("edges.txt") as f:
    for line in f:
        u,v,w = line.strip().split()
        G.add_edge(int(u), int(v), weight=float(w))

pos = nx.spring_layout(G, seed=42)
edge_labels = nx.get_edge_attributes(G, 'weight')

# ----------------- Load MSTs -----------------
def load_mst(filename):
    mst_edges = []
    with open(filename) as f:
        for line in f:
            u,v,w = line.strip().split()
            mst_edges.append((int(u),int(v)))
    return mst_edges

prim_edges = load_mst("prim_mst.txt")
kruskal_edges = load_mst("kruskal_mst.txt")

# ----------------- Draw Original Graph -----------------
plt.figure(figsize=(8,6))
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500)
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
plt.title("Original Graph")
plt.show()

# ----------------- Draw Prim MST -----------------
plt.figure(figsize=(8,6))
nx.draw(G, pos, with_labels=True, node_color='lightgreen', node_size=500)
nx.draw_networkx_edges(G, pos, edgelist=prim_edges, width=3, edge_color='red')
plt.title("Prim's MST")
plt.show()

# ----------------- Draw Kruskal MST -----------------
plt.figure(figsize=(8,6))
nx.draw(G, pos, with_labels=True, node_color='lightyellow', node_size=500)
nx.draw_networkx_edges(G, pos, edgelist=kruskal_edges, width=3, edge_color='blue')
plt.title("Kruskal's MST")
plt.show()
