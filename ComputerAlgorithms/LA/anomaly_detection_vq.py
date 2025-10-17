#!/usr/bin/env python3
"""
Anomaly Detection using Vector Quantization (VQ)
------------------------------------------------

Usage:
    python3 anomaly_detection_vq.py --train normal.csv --test test.csv --k 16 --threshold 0.05

Requirements:
    pip install numpy scikit-learn matplotlib pandas
"""

import numpy as np
import pandas as pd
import argparse
from sklearn.cluster import KMeans
from sklearn.metrics import pairwise_distances_argmin_min
import matplotlib.pyplot as plt

# -----------------------------------------
# Step 1: Train codebook on normal data
# -----------------------------------------
def train_codebook(train_data, k):
    print(f"[INFO] Training codebook with {k} codewords on {train_data.shape[0]} samples ...")
    kmeans = KMeans(n_clusters=k, n_init=10, random_state=42)
    kmeans.fit(train_data)
    print("[INFO] Codebook training completed.")
    return kmeans

# -----------------------------------------
# Step 2: Compute quantization errors
# -----------------------------------------
def compute_quantization_error(model, data):
    _, distances = pairwise_distances_argmin_min(data, model.cluster_centers_)
    return distances  # Euclidean distance to nearest codeword

# -----------------------------------------
# Step 3: Detect anomalies
# -----------------------------------------
def detect_anomalies(errors, threshold):
    anomalies = errors > threshold
    return anomalies

# -----------------------------------------
# Step 4: Visualize results
# -----------------------------------------
def visualize_errors(errors, anomalies=None, threshold=None):
    plt.figure(figsize=(10,5))
    plt.plot(errors, label="Quantization Error")
    if anomalies is not None:
        plt.scatter(np.where(anomalies)[0], errors[anomalies], color='red', label="Anomalies", zorder=5)
    if threshold is not None:
        plt.axhline(threshold, color='orange', linestyle='--', label=f"Threshold = {threshold:.3f}")
    plt.xlabel("Sample Index")
    plt.ylabel("Quantization Error")
    plt.title("Vector Quantization Anomaly Detection")
    plt.legend()
    plt.tight_layout()
    plt.show()

# -----------------------------------------
# Main pipeline
# -----------------------------------------
def main():
    parser = argparse.ArgumentParser(description="Anomaly Detection using Vector Quantization (VQ)")
    parser.add_argument('--train', required=True, help='Path to normal data (CSV)')
    parser.add_argument('--test', required=True, help='Path to test data (CSV)')
    parser.add_argument('--k', type=int, default=16, help='Codebook size (number of clusters)')
    parser.add_argument('--threshold', type=float, default=None, help='Error threshold for anomalies (auto if not set)')
    parser.add_argument('--vis', action='store_true', help='Visualize results')
    args = parser.parse_args()

    # Load data
    train_df = pd.read_csv(args.train)
    test_df = pd.read_csv(args.test)

    X_train = train_df.values
    X_test = test_df.values

    # Train codebook
    model = train_codebook(X_train, args.k)

    # Compute quantization errors
    errors = compute_quantization_error(model, X_test)

    # Auto threshold = mean + 2 * std (if not specified)
    if args.threshold is None:
        args.threshold = np.mean(errors) + 2 * np.std(errors)
        print(f"[INFO] Auto threshold set to {args.threshold:.4f}")

    anomalies = detect_anomalies(errors, args.threshold)

    # Print summary
    print("\n=== Detection Summary ===")
    print(f"Total samples: {len(errors)}")
    print(f"Anomalies detected: {np.sum(anomalies)}")
    print(f"Anomaly percentage: {100*np.mean(anomalies):.2f}%")

    # Optional visualization
    if args.vis:
        visualize_errors(errors, anomalies, args.threshold)

if __name__ == "__main__":
    main()


#pip install numpy scikit-learn matplotlib pandas
#python3 anomaly_detection_vq.py --train data/normal.csv --test data/test.csv --k 3 --vis

#python3 anomaly_detection_vq.py --train normal.csv --test test.csv --k 16 --threshold 0.1 --vis