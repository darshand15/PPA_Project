import matplotlib.pyplot as plt

# Given data
sequential_time = 5460901  # nanoseconds
parallel_times = [
    6534284,   # 1 thread
    10056580,  # 2 threads
    13086180,  # 4 threads
    15742792,  # 8 threads
    20936216,  # 16 threads
    23590524   # 32 threads
]

# Thread counts
thread_counts = [1, 2, 4, 8, 16, 32]

# Compute speedups
speedups = [sequential_time / pt for pt in parallel_times]

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(thread_counts, speedups, marker='o', linestyle='-', color='blue')
plt.title('Select Cols Cond Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')
plt.grid(True)
plt.xticks(thread_counts)
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/select_cols_cond.png', dpi=300)

# Show the plot
# plt.show()
