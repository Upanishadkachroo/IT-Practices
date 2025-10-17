#!/usr/bin/env python3
"""
Document clustering using K-Means and TF-IDF
--------------------------------------------

Usage:
    python document_clustering_kmeans.py --input_dir texts/ --k 5

Requirements:
    pip install scikit-learn matplotlib seaborn numpy pandas
"""

import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE
from sklearn.metrics import silhouette_score

# -------------------------------
# Utility functions
# -------------------------------

def load_documents(input_dir):
    docs, names = [], []
    for fname in os.listdir(input_dir):
        if fname.endswith(".txt"):
            with open(os.path.join(input_dir, fname), "r", encoding="utf-8", errors="ignore") as f:
                text = f.read().strip()
                docs.append(text)
                names.append(fname)
    return docs, names

def top_terms_per_cluster(vectorizer, kmeans, n_terms=10):
    feature_names = np.array(vectorizer.get_feature_names_out())
    centroids = kmeans.cluster_centers_
    top_terms = []
    for cluster_num, centroid in enumerate(centroids):
        top_indices = centroid.argsort()[-n_terms:][::-1]
        terms = feature_names[top_indices]
        top_terms.append((cluster_num, terms))
    return top_terms

def visualize_clusters(X, labels, names, k, method='pca'):
    if method == 'pca':
        reducer = PCA(n_components=2, random_state=42)
        reduced = reducer.fit_transform(X.toarray())
    else:
        reducer = TSNE(n_components=2, perplexity=15, random_state=42)
        reduced = reducer.fit_transform(X.toarray())

    plt.figure(figsize=(8,6))
    palette = sns.color_palette("tab10", k)
    sns.scatterplot(x=reduced[:,0], y=reduced[:,1], hue=labels, palette=palette, s=80, alpha=0.8, edgecolor='k')
    for i, name in enumerate(names):
        plt.text(reduced[i,0]+0.02, reduced[i,1]+0.02, name[:15], fontsize=8)
    plt.title(f"Document Clusters (k={k}, method={method.upper()})")
    plt.xlabel("Component 1")
    plt.ylabel("Component 2")
    plt.legend(title="Cluster", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.show()

# -------------------------------
# Main function
# -------------------------------

def main():
    parser = argparse.ArgumentParser(description="Document Clustering with K-Means")
    parser.add_argument('--input_dir', '-i', required=True, help='Directory containing text files')
    parser.add_argument('--k', type=int, default=5, help='Number of clusters')
    parser.add_argument('--max_features', type=int, default=2000, help='Max TF-IDF features')
    parser.add_argument('--vis', action='store_true', help='Show 2D visualization')
    parser.add_argument('--method', choices=['pca','tsne'], default='pca', help='Visualization method')
    args = parser.parse_args()

    docs, names = load_documents(args.input_dir)
    print(f"Loaded {len(docs)} documents from {args.input_dir}")

    # Step 1: TF-IDF vectorization
    vectorizer = TfidfVectorizer(stop_words='english', max_features=args.max_features)
    X = vectorizer.fit_transform(docs)

    # Step 2: K-Means clustering
    print(f"Clustering into {args.k} clusters ...")
    kmeans = KMeans(n_clusters=args.k, n_init=10, random_state=42)
    kmeans.fit(X)
    labels = kmeans.labels_

    # Step 3: Evaluate & summarize
    sil = silhouette_score(X, labels)
    print(f"\nSilhouette score: {sil:.3f}")

    print("\nTop keywords per cluster:")
    top_terms = top_terms_per_cluster(vectorizer, kmeans, n_terms=10)
    for cluster_id, terms in top_terms:
        print(f"Cluster {cluster_id}: {', '.join(terms)}")

    print("\nDocument assignments:")
    for i, label in enumerate(labels):
        print(f"[Cluster {label}] {names[i]}")

    # Step 4: Optional visualization
    if args.vis:
        visualize_clusters(X, labels, names, args.k, args.method)

if __name__ == "__main__":
    main()



#python document_clustering_kmeans.py --input_dir texts/ --k 3 --vis --method tsne
