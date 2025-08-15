# rsa_no_pandas.py

# Function to calculate gcd
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# Function to calculate modular inverse
def mod_inverse(e, phi):
    for d in range(3, phi):
        if (e * d) % phi == 1:
            return d
    return None

# Function to perform encryption
def encrypt(m, e, n):
    return pow(m, e, n)

# Function to perform decryption
def decrypt(c, d, n):
    return pow(c, d, n)

# RSA parameter setup
p = 11
q = 13
n = p * q
phi = (p - 1) * (q - 1)

# Generate possible e, d pairs
pairs = []
for e in range(3, phi):
    if gcd(e, phi) == 1:
        d = mod_inverse(e, phi)
        if d:
            pairs.append((e, d))

# Print table header
print(f"{'e':<5}{'d':<5}{'n':<5}{'phi(n)':<8}")
print("-" * 25)

# Print table rows
for e, d in pairs:
    print(f"{e:<5}{d:<5}{n:<5}{phi:<8}")

# Choose plaintext
plaintext = 7
print("\nChosen plaintext:", plaintext)

# Demonstrate encryption/decryption for first two pairs
print("\nEncryption/Decryption Demonstration:")
for i, (e, d) in enumerate(pairs[:2], start=1):
    c = encrypt(plaintext, e, n)
    m = decrypt(c, d, n)
    print(f"Pair {i} -> e={e}, d={d} | Cipher={c} | Decrypted={m}")

# Test cross pairs (e1,d2), (e2,d1)
print("\nCross Pair Testing:")
if len(pairs) >= 2:
    e1, d1 = pairs[0]
    e2, d2 = pairs[1]
    
    # (e1, d2)
    c = encrypt(plaintext, e1, n)
    m = decrypt(c, d2, n)
    print(f"(e1={e1}, d2={d2}) -> Cipher={c} | Decrypted={m}")
    
    # (e2, d1)
    c = encrypt(plaintext, e2, n)
    m = decrypt(c, d1, n)
    print(f"(e2={e2}, d1={d1}) -> Cipher={c} | Decrypted={m}")

