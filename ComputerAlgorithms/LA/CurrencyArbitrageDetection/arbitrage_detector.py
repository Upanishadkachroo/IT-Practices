#!/usr/bin/env python3
"""
Currency arbitrage detector using Bellman-Ford on -log(exchange_rate).

Usage:
    python3 arbitrage_detector.py --rates rates.csv --visualize

Outputs:
 - prints any arbitrage cycle found and its profit factor
 - writes graph files edges_for_sage.txt and cycle_nodes.txt
 - (optional) shows a Matplotlib/NetworkX visualization
"""

import math
import csv
import argparse
from collections import defaultdict

# optional visualization imports
try:
    import networkx as nx
    import matplotlib.pyplot as plt
    HAVE_NX = True
except Exception:
    HAVE_NX = False

def read_rates(path):
    edges = []
    nodes = set()
    with open(path, newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            if not row or row[0].strip().startswith('#'):
                continue
            src = row[0].strip()
            dst = row[1].strip()
            rate = float(row[2])
            edges.append((src, dst, rate))
            nodes.add(src); nodes.add(dst)
    return list(nodes), edges

def build_graph(nodes, edges):
    idx = {n:i for i,n in enumerate(nodes)}
    inv = {i:n for n,i in idx.items()}
    weighted_edges = []
    for (u,v,r) in edges:
        if r <= 0:
            continue
        w = -math.log(r)
        weighted_edges.append((idx[u], idx[v], w))
    return idx, inv, weighted_edges

def bellman_ford_find_negative_cycle(n, edges):
    # edges: list of (u,v,w)
    # returns cycle as list of nodes (indices) or None
    dist = [0.0]*n
    parent = [-1]*n

    # run n-1 relaxations (we initialize dist to 0 to allow detection from any start)
    for i in range(n-1):
        updated = False
        for (u,v,w) in edges:
            if dist[u] + w < dist[v] - 1e-15:
                dist[v] = dist[u] + w
                parent[v] = u
                updated = True
        if not updated:
            break

    # one more pass to detect negative cycle
    for (u,v,w) in edges:
        if dist[u] + w < dist[v] - 1e-15:
            # negative cycle found; trace back
            # to ensure we are inside the cycle, walk parent chain n times
            x = v
            for _ in range(n):
                x = parent[x] if parent[x] != -1 else x
            # now collect the cycle
            cycle = []
            cur = x
            while True:
                cycle.append(cur)
                cur = parent[cur]
                if cur == -1:
                    break
                if cur == x and len(cycle) > 0:
                    break
            cycle.reverse()
            return cycle
    return None

def cycle_profit_and_path(cycle, inv, edges_map):
    # cycle: list of indices [i0, i1, i2, ..., i0]
    # edges_map maps (u_idx,v_idx) -> rate
    path = []
    prod = 1.0
    for i in range(len(cycle)):
        u = cycle[i]
        v = cycle[(i+1)%len(cycle)]
        rate = edges_map.get((u,v), None)
        if rate is None:
            # should not happen
            rate = math.exp(-edges_map.get((u,v), 0.0))
        prod *= rate
        path.append((inv[u], inv[v], rate))
    return prod, path

def prepare_edges_map(edges, idx):
    edges_map = {}
    for u_name, v_name, rate in edges:
        # edges list uses names, idx maps names->indices
        u = idx[u_name]
        v = idx[v_name]
        edges_map[(u,v)] = rate
    return edges_map

def save_for_sage(edges, cycle_nodes, inv, fname_edges='edges_for_sage.txt', fname_cycle='cycle_nodes.txt'):
    # edges: list of (u_name,v_name,rate)
    # cycle_nodes: list of indices
    with open(fname_edges, 'w') as f:
        for (u_name,v_name,rate) in edges:
            f.write(f"{u_name} {v_name} {rate}\n")
    with open(fname_cycle, 'w') as f:
        for idx in cycle_nodes:
            f.write(f"{inv[idx]}\n")

def visualize_with_networkx(edges, cycle_path=None):
    if not HAVE_NX:
        print("NetworkX/matplotlib not available; skip visualization.")
        return
    G = nx.DiGraph()
    for u,v,r in edges:
        G.add_edge(u, v, weight=r)
    pos = nx.spring_layout(G, seed=42)
    # draw nodes and edges
    plt.figure(figsize=(10,7))
    nx.draw_networkx_nodes(G, pos, node_size=800)
    nx.draw_networkx_labels(G, pos)
    edge_labels = {(u,v): f"{r:.4g}" for (u,v,r) in edges}
    nx.draw_networkx_edges(G, pos, edge_color='gray', arrowsize=20)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

    if cycle_path:
        # highlight cycle edges in red
        cyc_edges = [(cycle_path[i][0], cycle_path[i][1]) for i in range(len(cycle_path))]
        nx.draw_networkx_edges(G, pos, edgelist=cyc_edges, edge_color='red', width=3, arrowsize=20)
    plt.title("Currency Graph (rates). Red = detected arbitrage cycle")
    plt.axis('off')
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Currency arbitrage detector")
    parser.add_argument('--rates', default='rates.csv', help='CSV file: src,dst,rate (no header)')
    parser.add_argument('--visualize', action='store_true', help='Show NetworkX visualization (if available)')
    args = parser.parse_args()

    nodes, edges = read_rates(args.rates)
    if len(nodes) == 0 or len(edges) == 0:
        print("No nodes or edges read. Check rates.csv format.")
        return

    idx, inv, weighted_edges = build_graph(nodes, edges)
    # run Bellman-Ford
    n = len(nodes)
    cycle = bellman_ford_find_negative_cycle(n, weighted_edges)
    edges_map = prepare_edges_map(edges, idx)
    save_for_sage(edges, cycle if cycle else [], inv)  # save names and cycle for Sage or other tools

    if cycle is None:
        print("No arbitrage cycle found.")
    else:
        # convert cycle indices to names (ensure cycle is closed)
        cyc_names = [inv[i] for i in cycle]
        # make closed by repeating first
        cyc_indices_closed = cycle[:]  # indices
        # compute profit along cycle
        profit = 1.0
        path_entries = []
        for i in range(len(cycle)):
            u = cycle[i]
            v = cycle[(i+1)%len(cycle)]
            u_name = inv[u]; v_name = inv[v]
            rate = edges_map.get((u,v), None)
            if rate is None:
                # maybe the cycle was detected via different orientation; try to find available edge
                print(f"Edge {u_name}->{v_name} not found in input rates.")
                rate = 0.0
            profit *= rate
            path_entries.append((u_name, v_name, rate))
        print("Arbitrage cycle detected:")
        for (u,v,r) in path_entries:
            print(f"  {u} -> {v} @ {r}")
        print(f"Cycle nodes: {' -> '.join([inv[i] for i in cycle] + [inv[cycle[0]]])}")
        print(f"Profit factor (product of rates) = {profit:.6f}")
        if profit > 1.0:
            print(f"Arbitrage exists: net gain {100*(profit-1):.3f}%")
        else:
            print("Detected negative cycle but product <= 1. (Numerical issues?)")

        # prepare cycle path for visualization (list of tuples for edges)
        cycle_path = []
        for i in range(len(cycle)):
            u = inv[cycle[i]]
            v = inv[cycle[(i+1)%len(cycle)]]
            cycle_path.append((u,v, edges_map.get((cycle[i], cycle[(i+1)%len(cycle)]), 0.0)))

        if args.visualize:
            visualize_with_networkx(edges, cycle_path)

if __name__ == "__main__":
    main()
