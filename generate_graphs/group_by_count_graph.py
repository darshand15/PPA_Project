import matplotlib.pyplot as plt

# Given data
sequential_time = 14658555  # nanoseconds

# Parallel execution times for each thread count (Version 1 and Version 2)
parallel_v1_times = [
    14525955,  # 1 thread
    7546431,   # 2 threads
    4613332,   # 4 threads
    3280320,   # 8 threads
    2442168,   # 16 threads
    2388399    # 32 threads
]

parallel_v2_times = [
    17410500,  # 1 thread
    8637613,   # 2 threads
    5036981,   # 4 threads
    3602305,   # 8 threads
    2722171,   # 16 threads
    2617563    # 32 threads
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
plt.title('Group by Count Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')

# Grid, Ticks, and Legends
plt.grid(True)
plt.xticks(thread_counts)
plt.legend()

# Layout Adjustment
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/group_by_count.png', dpi=300)

# Show the plot
# plt.show()
