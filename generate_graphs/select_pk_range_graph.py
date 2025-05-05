import matplotlib.pyplot as plt

# Updated time data
sequential_times = [
    759451,
    772106,
    772806,
    759204,
    765120,
    766630
]

btree_times = [
    422897,
    449411,
    435169,
    433409,
    463427,
    444547
]

# Plotting
plt.figure(figsize=(10, 6))
x = list(range(1, len(sequential_times) + 1))

plt.plot(x, sequential_times, marker='o', label='Sequential Range Time')
plt.plot(x, btree_times, marker='s', label='B-Tree Range Time')

plt.title('Select Range Query Comparison: Sequential vs B-Tree (~2x improvement)', fontweight='bold')
plt.xlabel('Execution Number', fontweight='bold')
plt.ylabel('Time Taken (in nanoseconds)', fontweight='bold')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Save the plot as a PNG file
plt.savefig('./graphs/select_pk_range.png', dpi=300)

# Show the plot
# plt.show()
