import matplotlib.pyplot as plt

# Given data
sequential_time = 14228056  # nanoseconds

# Parallel execution times for each thread count (Version 1 and Version 2)
parallel_v1_times = [
    14537624,  # 1 thread
    7494801,   # 2 threads
    4807647,   # 4 threads
    3483091,   # 8 threads
    2700204,   # 16 threads
    2388501    # 32 threads
]

parallel_v2_times = [
    17477068,  # 1 thread
    8478224,   # 2 threads
    4698495,   # 4 threads
    2894469,   # 8 threads
    1860128,   # 16 threads
    1717681    # 32 threads
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
plt.title('Group by Sum Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')

# Grid, Ticks, and Legends
plt.grid(True)
plt.xticks(thread_counts)
plt.legend()

# Layout Adjustment
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/group_by_sum.png', dpi=300)

# Show the plot
# plt.show()
