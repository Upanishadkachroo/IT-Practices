import math
import csv

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

with open("affine_table.csv", "w", newline="") as file:
    writer = csv.writer(file)

    # Header row
    header = ["a", "b"] + [f"p={p}" for p in range(101)]
    writer.writerow(header)

    for a in range(1, 51):
        if math.gcd(a, 26) == 1:  # only valid 'a'
            for b in range(1, 51):
                row = [a, b]
                for p in range(101):
                    c = (a * p + b) % 26
                    row.append(c)
                writer.writerow(row)

print("Table saved as affine_table.csv")
