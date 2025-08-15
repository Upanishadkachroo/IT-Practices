import math
import pandas as pd
from itertools import product

# ------------------- Helper functions -------------------

def is_prime(n):
    """Check if a number is prime."""
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    for f in range(3, int(n**0.5) + 1, 2):
        if n % f == 0:
            return False
    return True

def egcd(a, b):
    """Extended Euclidean Algorithm."""
    if b == 0:
        return (a, 1, 0)
    g, x1, y1 = egcd(b, a % b)
    return (g, y1, x1 - (a // b) * y1)

def modinv(a, m):
    """Find modular inverse of a mod m."""
    g, x, _ = egcd(a, m)
    if g != 1:
        return None
    return x % m

# ------------------- Step 1: Generate RSA table -------------------

primes = [x for x in range(2, 101) if is_prime(x)]
rows = []

for p, q in product(primes, primes):
    if p == q:
        continue
    n = p * q
    phi = (p - 1) * (q - 1)
    for e in range(2, 51):
        if e >= phi:
            continue
        if math.gcd(e, phi) != 1:
            continue
        d = modinv(e, phi)
        if d is not None and 1 <= d <= 50:
            rows.append({"p": p, "q": q, "n": n, "phi_n": phi, "e": e, "d": d})

df = pd.DataFrame(rows).sort_values(["p", "q", "e", "d"]).reset_index(drop=True)

# Save the table
df.to_csv("rsa_valid_pairs_full_table.csv", index=False)
print(f"Generated table with {len(df)} rows and saved to 'rsa_valid_pairs_full_table.csv'")

# ------------------- Step 2: Pick a (p,q) and demonstrate -------------------

# Find a (p,q) with at least 2 key pairs
multi = df.groupby(["p","q"]).size().reset_index(name="count")
multi_pairs = multi[multi["count"] >= 2]

if multi_pairs.empty:
    raise RuntimeError("No (p,q) found with >= 2 valid (e,d) pairs.")

# Pick the first such (p,q)
p, q = int(multi_pairs.iloc[0].p), int(multi_pairs.iloc[0].q)
same_pq = df[(df["p"] == p) & (df["q"] == q)].reset_index(drop=True)

# Select two key pairs
e1, d1 = int(same_pq.iloc[0].e), int(same_pq.iloc[0].d)
e2, d2 = int(same_pq.iloc[1].e), int(same_pq.iloc[1].d)
n = p * q

print(f"\nChosen (p,q)=({p},{q}), n={n}, phi(n)={(p-1)*(q-1)}")
print(f"Key pair 1: (e1={e1}, d1={d1})")
print(f"Key pair 2: (e2={e2}, d2={d2})")

# ------------------- Step 3: Demonstrate encryption/decryption -------------------

plaintexts = [2, 17, 42]  # choose any m < n
print("\nResults:")

for m in plaintexts:
    if not (0 < m < n):
        continue
    # Correct pair (e1,d1)
    c1 = pow(m, e1, n)
    dec11 = pow(c1, d1, n)
    # Mismatched (e1,d2)
    dec12 = pow(c1, d2, n)
    # Mismatched (e2,d1)
    c2 = pow(m, e2, n)
    dec21 = pow(c2, d1, n)
    # Correct pair (e2,d2)
    dec22 = pow(c2, d2, n)

    print(f"\nPlaintext m = {m}")
    print(f"  Encrypt with e1={e1} -> Ciphertext c1={c1}")
    print(f"    Decrypt with d1={d1} -> {dec11}  | Correct: {dec11==m}")
    print(f"    Decrypt with d2={d2} -> {dec12}  | Correct: {dec12==m}")
    print(f"  Encrypt with e2={e2} -> Ciphertext c2={c2}")
    print(f"    Decrypt with d1={d1} -> {dec21}  | Correct: {dec21==m}")
    print(f"    Decrypt with d2={d2} -> {dec22}  | Correct: {dec22==m}")

