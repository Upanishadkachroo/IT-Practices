import matplotlib.pyplot as plt
import networkx as nx

# Load graph edges and communities
edges = []
communities = {}
with open("output/graph.txt") as f:
    for line in f:
        u,v,c = map(int, line.strip().split())
        edges.append((u,v))
        communities[u] = c
        communities[v] = c

# Load betweenness centrality
bc = {}
with open("output/centrality.txt") as f:
    for line in f:
        node, val = line.strip().split()
        bc[int(node)] = float(val)

# Build NetworkX graph
G = nx.Graph()
G.add_edges_from(edges)

# Node colors by community
colors = [communities[node] for node in G.nodes()]

# Node sizes by centrality
node_sizes = [500*bc[node] for node in G.nodes()]

# Layout and draw
pos = nx.spring_layout(G, seed=42)
nx.draw(G, pos, node_color=colors, node_size=node_sizes,
        with_labels=True, cmap=plt.cm.tab20)
plt.title("Social Network: Communities and Centrality")
plt.show()
