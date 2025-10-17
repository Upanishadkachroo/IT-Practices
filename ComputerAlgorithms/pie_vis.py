import matplotlib.pyplot as plt

names = ["operator[]", "std::move", "std::swap", "partition", "quickSort"]
calls = [4326096, 3235938, 1078646, 66662, 133325]

plt.figure(figsize=(12,5))

# Pie chart
plt.subplot(1,2,1)
plt.pie(calls, labels=names, autopct='%1.1f%%', startangle=140)
plt.title("Call-count distribution (Pie Chart)")
plt.axis('equal')

# Bar chart
plt.subplot(1,2,2)
plt.bar(names, calls, color="skyblue", edgecolor="black")
plt.title("Number of Calls (Bar Chart)")
plt.xlabel("Functions")
plt.ylabel("Number of Calls")
plt.xticks(rotation=30)

plt.tight_layout()
plt.show()