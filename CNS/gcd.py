import matplotlib.pyplot as plt

# Example data (you can replace this with your actual data from the C++ output)
digits = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
operations = [5, 10, 12, 15, 20, 30, 40, 50, 60, 70]  # Example operations count

# Plotting the graph
plt.figure(figsize=(10, 6))
plt.plot(digits, operations, marker='o', linestyle='-', color='b')
plt.title("Average Floating Operations vs Number of Digits")
plt.xlabel("Number of Digits in Input Numbers")
plt.ylabel("Average Number of Operations")
plt.grid(True)
plt.show()
