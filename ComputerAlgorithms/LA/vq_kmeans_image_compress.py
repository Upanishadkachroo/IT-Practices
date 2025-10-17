#!/usr/bin/env python3
"""
Vector Quantization (k-means) image compression for grayscale images.

Usage:
    python vq_kmeans_image_compress.py --input image.jpg --ks 16 32 64 --patch 2 --maxiter 100 --out_prefix result

Produces:
 - Reconstructed images for each k
 - A plot of PSNR vs compression ratio
 - Prints detailed metrics
"""

import argparse
import numpy as np
from PIL import Image
import math
import os
import sys
import matplotlib.pyplot as plt
from time import time

# Optional: if skimage available, use structural_similarity for SSIM
try:
    from skimage.metrics import structural_similarity as ssim
    SKIMAGE = True
except Exception:
    SKIMAGE = False

# -----------------------
# Utilities
# -----------------------
def to_grayscale_array(img_path):
    img = Image.open(img_path).convert('L')
    arr = np.array(img, dtype=np.uint8)
    return arr

def patches_from_image(im_arr, patch_size):
    h, w = im_arr.shape
    ph = patch_size * (h // patch_size)
    pw = patch_size * (w // patch_size)
    im_trim = im_arr[:ph, :pw]
    # reshape to patches
    patches = im_trim.reshape(ph // patch_size, patch_size, pw // patch_size, patch_size)
    # reorder to (num_patches, patch_size, patch_size)
    patches = patches.swapaxes(1,2).reshape(-1, patch_size, patch_size)
    vectors = patches.reshape(patches.shape[0], -1).astype(np.float32)
    return vectors, (ph, pw)

def reconstruct_image_from_patches(codebook, indices, image_shape, patch_size):
    num_patches = indices.shape[0]
    ph, pw = image_shape
    patches = codebook[indices].reshape(num_patches, patch_size, patch_size)
    # reconstruct grid
    rows = ph // patch_size
    cols = pw // patch_size
    patches = patches.reshape(rows, cols, patch_size, patch_size).swapaxes(1,2)
    im_rec = patches.reshape(ph, pw)
    return np.clip(im_rec, 0, 255).astype(np.uint8)

def psnr(original, reconstructed):
    # only compare same-sized region (trimmed to reconstructed size)
    orig = original[:reconstructed.shape[0], :reconstructed.shape[1]].astype(np.float64)
    rec = reconstructed.astype(np.float64)
    mse = np.mean((orig - rec) ** 2)
    if mse == 0:
        return float('inf')
    PIXEL_MAX = 255.0
    return 20 * math.log10(PIXEL_MAX / math.sqrt(mse))

def bytes_for_codebook(k, D):
    # assuming storing as raw bytes (8 bits per value)
    return k * D  # bytes

def bits_for_indices(num_patches, k):
    bits_per_index = math.ceil(math.log2(k))
    return num_patches * bits_per_index

def theoretical_compression_ratio(original_shape, k, D, num_patches, patch_size):
    orig_bits = original_shape[0] * original_shape[1] * 8
    cb_bytes = bytes_for_codebook(k, D)
    cb_bits = cb_bytes * 8
    idx_bits = bits_for_indices(num_patches, k)
    compressed_bits = cb_bits + idx_bits
    return orig_bits / compressed_bits, orig_bits, compressed_bits

# -----------------------
# Simple k-means (numpy) with kmeans++ init
# -----------------------
def kmeans_plus_plus_init(X, k, random_state=None):
    N, D = X.shape
    rng = np.random.RandomState(random_state)
    centers = np.empty((k, D), dtype=X.dtype)
    # pick first center randomly
    i = rng.randint(N)
    centers[0] = X[i]
    # distances squared to nearest center
    closest_sq = np.sum((X - centers[0])**2, axis=1)
    for c in range(1, k):
        probs = closest_sq / closest_sq.sum()
        r = rng.rand()
        idx = np.searchsorted(np.cumsum(probs), r)
        centers[c] = X[idx]
        new_sq = np.sum((X - centers[c])**2, axis=1)
        closest_sq = np.minimum(closest_sq, new_sq)
    return centers

def kmeans(X, k, max_iter=100, tol=1e-4, random_state=None, verbose=False):
    N, D = X.shape
    centers = kmeans_plus_plus_init(X, k, random_state=random_state)
    labels = np.zeros(N, dtype=np.int32)
    for it in range(max_iter):
        # assign
        dists = np.sum((X[:, None, :] - centers[None, :, :])**2, axis=2)  # N x k
        new_labels = np.argmin(dists, axis=1)
        # update
        changed = np.sum(new_labels != labels)
        labels = new_labels
        old_centers = centers.copy()
        for j in range(k):
            pts = X[labels == j]
            if pts.shape[0] > 0:
                centers[j] = pts.mean(axis=0)
            # if empty cluster, reinitialize randomly
            else:
                centers[j] = X[np.random.randint(N)]
        center_shift = np.sqrt(np.sum((centers - old_centers)**2, axis=1)).max()
        if verbose:
            print(f"iter {it:3d} changed={changed} shift={center_shift:.6f}")
        if center_shift <= tol:
            break
    return centers, labels

# -----------------------
# Main driver: compress for given k
# -----------------------
def compress_with_vq(img_path, k, patch_size=2, max_iter=100, random_state=None, verbose=False):
    arr = to_grayscale_array(img_path)
    H, W = arr.shape
    vectors, (ph, pw) = patches_from_image(arr, patch_size)
    num_patches = vectors.shape[0]
    D = vectors.shape[1]
    t0 = time()
    centers, labels = kmeans(vectors, k, max_iter=max_iter, random_state=random_state, verbose=verbose)
    t1 = time()
    reconstructed = reconstruct_image_from_patches(centers, labels, (ph, pw), patch_size)
    psnr_val = psnr(arr, reconstructed)
    if SKIMAGE:
        ssim_val = ssim(arr[:ph, :pw], reconstructed, data_range=255)
    else:
        ssim_val = None
    # theoretical compression
    ratio, orig_bits, comp_bits = theoretical_compression_ratio((H,W), k, D, num_patches, patch_size)
    results = {
        'k': k,
        'patch_size': patch_size,
        'D': D,
        'num_patches': num_patches,
        'time_sec': t1 - t0,
        'psnr': psnr_val,
        'ssim': ssim_val,
        'orig_bits': orig_bits,
        'compressed_bits': comp_bits,
        'compression_ratio': ratio,
        'reconstructed_image': reconstructed,
        'codebook': centers,
        'labels': labels
    }
    return results

# -----------------------
# Command line runner
# -----------------------
def parse_args():
    p = argparse.ArgumentParser(description="Vector Quantization (k-means) image compression")
    p.add_argument('--input', '-i', required=True, help='Input image path')
    p.add_argument('--ks', '-k', nargs='+', type=int, default=[16, 32, 64], help='List of k values to try')
    p.add_argument('--patch', type=int, default=2, help='Patch size (patch x patch)')
    p.add_argument('--maxiter', type=int, default=100, help='Max iterations for k-means')
    p.add_argument('--out_prefix', type=str, default='vq_result', help='Output prefix for images and plot')
    p.add_argument('--random_state', type=int, default=42)
    p.add_argument('--verbose', action='store_true')
    return p.parse_args()

def main():
    args = parse_args()
    img_path = args.input
    ks = args.ks
    patch_size = args.patch
    results_list = []
    arr = to_grayscale_array(img_path)
    H, W = arr.shape
    print(f"Image: {img_path} -> size {W}x{H}, patch_size={patch_size}")
    for k in ks:
        print(f"\nRunning k={k} ...")
        res = compress_with_vq(img_path, k, patch_size=patch_size, max_iter=args.maxiter, random_state=args.random_state, verbose=args.verbose)
        results_list.append(res)
        # Save reconstructed image
        out_img = Image.fromarray(res['reconstructed_image'])
        fname = f"{args.out_prefix}_k{res['k']}_patch{patch_size}.png"
        out_img.save(fname)
        print(f"Saved reconstructed image -> {fname}")
        print(f" k={res['k']}  | PSNR={res['psnr']:.3f}  | SSIM={res['ssim'] if res['ssim'] is not None else 'n/a'}")
        print(f" Orig bits={res['orig_bits']}  Comp bits={res['compressed_bits']}  Ratio={res['compression_ratio']:.2f}")
        print(f" Time: {res['time_sec']:.3f}s  #patches={res['num_patches']}  D={res['D']}")

    # Plot PSNR vs Compression Ratio
    ratios = [r['compression_ratio'] for r in results_list]
    psnrs = [r['psnr'] for r in results_list]
    ks_list = [r['k'] for r in results_list]

    plt.figure(figsize=(7,5))
    plt.plot(ratios, psnrs, marker='o')
    for x,y,k in zip(ratios, psnrs, ks_list):
        plt.text(x, y, f"k={k}")
    plt.xlabel('Theoretical Compression Ratio (orig bits / compressed bits)')
    plt.ylabel('PSNR (dB)')
    plt.title(f'VQ Compression: PSNR vs Compression Ratio (patch={patch_size}x{patch_size})')
    plt.grid(True)
    plotname = f"{args.out_prefix}_psnr_vs_ratio_patch{patch_size}.png"
    plt.savefig(plotname, bbox_inches='tight', dpi=150)
    print(f"\nSaved plot -> {plotname}")
    plt.show()

if __name__ == "__main__":
    main()


#python vq_kmeans_image_compress.py --input my_image.jpg --ks 8 16 32 64 --patch 2 --maxiter 100 --out_prefix myvq
