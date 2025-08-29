#include <bits/stdc++.h>
using namespace std;

class KMeans {
    int k;                  // number of clusters
    int max_iters;          // maximum iterations
    double tol;             // tolerance for convergence
    vector<vector<double>> centroids;  // cluster centers

public:
    KMeans(int k, int max_iters = 100, double tol = 1e-4) {
        this->k = k;
        this->max_iters = max_iters;
        this->tol = tol;
    }

    // Euclidean distance
    double distance(const vector<double>& a, const vector<double>& b) {
        double sum = 0.0;
        for (size_t i = 0; i < a.size(); i++)
            sum += pow(a[i] - b[i], 2);
        return sqrt(sum);
    }

    // Assign each point to nearest cluster
    vector<int> assignClusters(const vector<vector<double>>& X) {
        vector<int> labels(X.size());
        for (size_t i = 0; i < X.size(); i++) {
            double min_dist = 1e9;
            int cluster = -1;
            for (int j = 0; j < k; j++) {
                double dist = distance(X[i], centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster = j;
                }
            }
            labels[i] = cluster;
        }
        return labels;
    }

    void fit(const vector<vector<double>>& X) {
        int n = X.size();

        // Step 1: Initialize centroids randomly
        srand(time(0));
        centroids.clear();
        unordered_set<int> chosen;
        while (centroids.size() < (size_t)k) {
            int idx = rand() % n;
            if (!chosen.count(idx)) {
                centroids.push_back(X[idx]);
                chosen.insert(idx);
            }
        }

        for (int it = 0; it < max_iters; it++) {
            // Step 2: Assign clusters
            vector<int> labels = assignClusters(X);

            // Step 3: Recompute centroids
            vector<vector<double>> new_centroids(k, vector<double>(X[0].size(), 0.0));
            vector<int> counts(k, 0);

            for (size_t i = 0; i < X.size(); i++) {
                int cluster = labels[i];
                counts[cluster]++;
                for (size_t j = 0; j < X[i].size(); j++)
                    new_centroids[cluster][j] += X[i][j];
            }

            for (int j = 0; j < k; j++) {
                if (counts[j] > 0) {
                    for (size_t d = 0; d < new_centroids[j].size(); d++)
                        new_centroids[j][d] /= counts[j];
                } else {
                    // if a cluster got no points, keep old centroid
                    new_centroids[j] = centroids[j];
                }
            }

            // Step 4: Check for convergence
            bool converged = true;
            for (int j = 0; j < k; j++) {
                if (distance(new_centroids[j], centroids[j]) > tol) {
                    converged = false;
                    break;
                }
            }

            centroids = new_centroids;
            if (converged) break;
        }
    }

    int predict(const vector<double>& point) {
        double min_dist = 1e9;
        int cluster = -1;
        for (int j = 0; j < k; j++) {
            double dist = distance(point, centroids[j]);
            if (dist < min_dist) {
                min_dist = dist;
                cluster = j;
            }
        }
        return cluster;
    }

    void printCentroids() {
        cout << "Final Centroids:\n";
        for (int i = 0; i < k; i++) {
            cout << "Cluster " << i << ": ";
            for (double val : centroids[i]) cout << val << " ";
            cout << endl;
        }
    }
};

int main() {
    // Sample dataset (2D points)
    vector<vector<double>> X = {
        {1.0, 2.0}, {1.5, 1.8}, {5.0, 8.0},
        {8.0, 8.0}, {1.0, 0.6}, {9.0, 11.0},
        {8.0, 2.0}, {10.0, 2.0}, {9.0, 3.0}
    };

    int k = 3; // number of clusters
    KMeans kmeans(k);
    kmeans.fit(X);

    kmeans.printCentroids();

    // Predict for a new point
    vector<double> new_point = {2.0, 3.0};
    int cluster = kmeans.predict(new_point);
    cout << "Point {2,3} belongs to cluster: " << cluster << endl;

    return 0;
}

