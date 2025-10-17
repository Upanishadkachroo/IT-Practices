# visualize_arbitrage.sage
from sage.all import *

# load edges
edges = []
with open('edges_for_sage.txt') as f:
    for line in f:
        a,b,r = line.strip().split()
        edges.append( (a,b,float(r)) )

# build directed graph in sage
G = DiGraph()
for a,b,r in edges:
    G.add_edge(a,b, label=str(round(r,6)))

# load cycle nodes (if any)
cycle = []
try:
    with open('cycle_nodes.txt') as f:
        for line in f:
            cycle.append(line.strip())
except FileNotFoundError:
    cycle = []

# layout and plotting
pos = G.layout(graph_tool=False)  # default layout
p = G.plot(pos=pos, vertex_size=300, edge_labels=True)
if cycle:
    # highlight cycle edges
    cycle_edges = []
    for i in range(len(cycle)):
        u = cycle[i]
        v = cycle[(i+1) % len(cycle)]
        cycle_edges.append((u,v))
    # Overlay red thickness on cycle edges
    p2 = G.plot(pos=pos, edge_colors={e: 'red' if (e[0],e[1]) in cycle_edges else 'gray' for e in G.edges()}, 
                edge_width={e: 3 if (e[0],e[1]) in cycle_edges else 1 for e in G.edges()}, vertex_fill_color='lightblue')
    show(p2)
else:
    show(p)
