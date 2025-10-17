import matplotlib.pyplot as plt
import networkx as nx

# ----------------- Load Graph -----------------
G = nx.Graph()
with open("edges.txt") as f:
    for line in f:
        u,v,w = line.strip().split()
        G.add_edge(int(u), int(v), weight=float(w))

# Layout & draw graph
pos = nx.spring_layout(G, seed=42)
labels = nx.get_edge_attributes(G,'weight')
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500)
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
plt.title("Graph with Edge Weights")
plt.show()

# ----------------- Load Shortest Paths -----------------
paths = {}
with open("shortest_paths.txt") as f:
    src = None
    parent = {}
    for line in f:
        if line.startswith("Source"):
            if src is not None:
                paths[src] = parent.copy()
            src = int(line.strip().split(":")[1])
            parent.clear()
        elif line.strip():
            v,d,p = map(float, line.strip().split())
            parent[int(v)] = int(p)
    if src is not None:
        paths[src] = parent.copy()

# ----------------- Highlight shortest path (example: 0->5) -----------------
source = 0
target = 5
path = []
cur = target
while cur != -1:
    path.append(cur)
    cur = paths[source][cur]
path = path[::-1]

edge_list = [(path[i], path[i+1]) for i in range(len(path)-1)]
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500)
nx.draw_networkx_edges(G, pos, edgelist=edge_list, width=3, edge_color='red')
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)
plt.title(f"Shortest Path from {source} to {target}")
plt.show()
