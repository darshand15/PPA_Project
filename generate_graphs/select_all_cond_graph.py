import matplotlib.pyplot as plt

# Given times
sequential_time = 12437788  # constant
parallel_times = [
    13424832,  # 1 thread
    21437205,  # 2 threads
    20491251,  # 4 threads
    21804433,  # 8 threads
    24719290,  # 16 threads
    24894595   # 32 threads
]

# Corresponding number of threads
thread_counts = [1, 2, 4, 8, 16, 32]

# Calculate speedup
speedups = [sequential_time / pt for pt in parallel_times]

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(thread_counts, speedups, marker='o', linestyle='-', color='blue')
plt.title('Select All Cond Query Comparison: Sequential vs Parallel', fontweight='bold')
plt.xlabel('Number of Threads', fontweight='bold')
plt.ylabel('Speedup (Sequential / Parallel)', fontweight='bold')
plt.grid(True)
plt.xticks(thread_counts)
plt.tight_layout()

# Save the plot
plt.savefig('./graphs/select_all_cond.png', dpi=300)

# Show the plot
# plt.show()
