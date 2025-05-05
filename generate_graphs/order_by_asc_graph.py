import matplotlib.pyplot as plt

# Given data
sequential_time = 45102283  # nanoseconds

# Parallel execution times for each thread count
parallel_times = [
    39859169,  # 1 thread
    26345854,  # 2 threads
    22752505,  # 4 threads
    21028301,  # 8 threads
    20538958,  # 16 threads
    22799743   # 32 threads
]

# Thread counts
thread_counts = [1, 2, 4, 8, 16, 32]

# Compute speedups
speedups = [sequential_time / pt for pt in parallel_times]

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(thread_counts, speedups, marker='o', linestyle='-', color='blue')
plt.title('Order by Asc Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')
plt.grid(True)
plt.xticks(thread_counts)
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/order_by_asc.png', dpi=300)

# Show the plot
# plt.show()
