import matplotlib.pyplot as plt

# Given data
sequential_time = 13925847  # nanoseconds

# Parallel execution times for each thread count (Version 1 and Version 2)
parallel_v1_times = [
    14454043,  # 1 thread
    7626134,   # 2 threads
    4714976,   # 4 threads
    3559912,   # 8 threads
    2585584,   # 16 threads
    2581363    # 32 threads
]

parallel_v2_times = [
    17592924,  # 1 thread
    8728577,   # 2 threads
    5008321,   # 4 threads
    3212314,   # 8 threads
    2229355,   # 16 threads
    2158241    # 32 threads
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
plt.title('Group by Min Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')

# Grid, Ticks, and Legends
plt.grid(True)
plt.xticks(thread_counts)
plt.legend()

# Layout Adjustment
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/group_by_min.png', dpi=300)

# Show the plot
# plt.show()
