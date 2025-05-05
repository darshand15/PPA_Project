import matplotlib.pyplot as plt

# Grouped time data
sequential_times = [
    193048,
    176861,
    195317,
    180663,
    177892,
    177192
]

btree_times = [
    10137,
    9462,
    9608,
    9403,
    9823,
    9616
]

# Plotting
plt.figure(figsize=(10, 6))
x = list(range(1, len(sequential_times) + 1))

plt.plot(x, sequential_times, marker='o', label='Sequential Search Time')
plt.plot(x, btree_times, marker='s', label='B-Tree Search Time')

plt.title('Select Equality Query Comparison: Sequential vs B-Tree (~20x improvement)', fontweight='bold')
plt.xlabel('Execution Number', fontweight='bold')
plt.ylabel('Time Taken (in nanoseconds)', fontweight='bold')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Save the plot as a PNG file
plt.savefig('./graphs/select_pk_eq.png', dpi=300)

# Show the plot
# plt.show()
