import matplotlib.pyplot as plt

# Given data
sequential_time = 14005942  # nanoseconds

# Parallel execution times for each thread count (Version 1 and Version 2)
parallel_v1_times = [
    14221665,  # 1 thread
    7462017,   # 2 threads
    4726323,   # 4 threads
    3371093,   # 8 threads
    2595764,   # 16 threads
    2575038    # 32 threads
]

parallel_v2_times = [
    17264520,  # 1 thread
    8451165,   # 2 threads
    4749604,   # 4 threads
    2961512,   # 8 threads
    1837230,   # 16 threads
    1664582    # 32 threads
]

# Thread counts
thread_counts = [1, 2, 4, 8, 16, 32]

# Calculate speedups
speedup_v1 = [sequential_time / pt for pt in parallel_v1_times]
speedup_v2 = [sequential_time / pt for pt in parallel_v2_times]

# Plotting
plt.figure(figsize=(10, 6))

# Plot Parallel Version 1
plt.plot(thread_counts, speedup_v1, marker='o', linestyle='-', label='Parallel Version 1', color='blue')

# Plot Parallel Version 2
plt.plot(thread_counts, speedup_v2, marker='s', linestyle='--', label='Parallel Version 2', color='green')

# Title and Labels with Bold Text
plt.title('Group by Avg Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')

# Grid, Ticks, and Legends
plt.grid(True)
plt.xticks(thread_counts)
plt.legend()

# Layout Adjustment
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/group_by_avg.png', dpi=300)

# Show the plot
# plt.show()
