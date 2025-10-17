#!/usr/bin/env python3
"""
Color-Based Image Segmentation using K-Means Clustering
-------------------------------------------------------

Usage:
    python3 color_segmentation_kmeans.py --image path/to/image.jpg --k 5 --output segmented.png

Requirements:
    pip install opencv-python scikit-learn matplotlib numpy
"""

import argparse
import numpy as np
import cv2
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

# -------------------------------------
# Function: Apply K-Means to segment image
# -------------------------------------
def segment_image(image_path, k, output_path=None, show=True):
    # Step 1: Load image (BGR → RGB)
    img = cv2.imread(image_path)
    if img is None:
        raise ValueError(f"Cannot read image from {image_path}")
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    h, w, c = img_rgb.shape

    # Step 2: Reshape to (num_pixels, 3)
    pixels = img_rgb.reshape(-1, 3)

    # Step 3: Apply K-Means
    print(f"Clustering {pixels.shape[0]} pixels into {k} segments ...")
    kmeans = KMeans(n_clusters=k, n_init=10, random_state=42)
    kmeans.fit(pixels)
    labels = kmeans.labels_
    centers = np.uint8(kmeans.cluster_centers_)

    # Step 4: Replace pixel colors with their cluster center
    segmented_pixels = centers[labels.flatten()]
    segmented_img = segmented_pixels.reshape((h, w, 3))

    # Step 5: Visualization
    if show:
        plt.figure(figsize=(10,5))
        plt.subplot(1,2,1)
        plt.imshow(img_rgb)
        plt.title("Original Image")
        plt.axis("off")

        plt.subplot(1,2,2)
        plt.imshow(segmented_img)
        plt.title(f"Segmented (k={k})")
        plt.axis("off")

        plt.tight_layout()
        plt.show()

    # Step 6: Save if requested
    if output_path:
        cv2.imwrite(output_path, cv2.cvtColor(segmented_img, cv2.COLOR_RGB2BGR))
        print(f"Segmented image saved as {output_path}")

    return segmented_img

# -------------------------------------
# Main driver
# -------------------------------------
def main():
    parser = argparse.ArgumentParser(description="Color-based image segmentation using K-Means")
    parser.add_argument('--image', '-i', required=True, help='Path to input image file')
    parser.add_argument('--k', type=int, default=4, help='Number of color segments')
    parser.add_argument('--output', '-o', default=None, help='Output image file')
    parser.add_argument('--no-show', action='store_true', help='Disable visualization')
    args = parser.parse_args()

    segment_image(args.image, args.k, args.output, not args.no_show)

if __name__ == "__main__":
    main()

#python3 color_segmentation_kmeans.py --image sample.jpg --k 5 --output segmented.png
#python3 color_segmentation_kmeans.py --image flower.jpg --k 3
#python3 color_segmentation_kmeans.py --image flower.jpg --k 8
