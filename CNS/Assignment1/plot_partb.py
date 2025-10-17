import matplotlib.pyplot as plt

digits = []
avg_iterations = []

with open("part_b_results.csv", "r") as f:
    for line in f:
        # Skip empty lines or header lines
        if not line.strip() or line.startswith("SecondNumDigits"):
            continue
        try:
            d, avg = line.strip().split(",")
            digits.append(int(d))
            avg_iterations.append(float(avg))
        except ValueError:
            continue

plt.plot(digits, avg_iterations, marker='o', color='r')
plt.xlabel("Second Number Digits")
plt.ylabel("Average Iterations")
plt.title("Euclid's Algorithm - Part (b)")
plt.grid(True)
plt.show()
