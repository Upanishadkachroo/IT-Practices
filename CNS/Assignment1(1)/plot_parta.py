import matplotlib.pyplot as plt

digits = []
avg_iterations = []

with open("part_a_results.csv", "r") as f:
    for line in f:
        # Skip empty lines or header lines
        if not line.strip() or line.startswith("Digits"):
            continue
        try:
            n, avg = line.strip().split(",")
            digits.append(int(n))
            avg_iterations.append(float(avg))
        except ValueError:
            continue  # skip any malformed line

plt.plot(digits, avg_iterations, marker='o', color='b')
plt.xlabel("Number of Digits (n)")
plt.ylabel("Average Iterations")
plt.title("Euclid's Algorithm - Part (a)")
plt.grid(True)
plt.show()
