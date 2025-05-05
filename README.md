# PPA_Project

## Project Title: Design and Implementation of a Parallel Relational Database from scratch

## Project Team Members: 
* Darshan Dinesh Kumar (NetID: dd3888, University ID: N10942768)

## Summary

The project is an effort to design and implement a Relational Database from scratch and a subset of its many operations while incorporating the various concepts gleaned from our “Programming Parallel Algorithms” course and related ideas inherent to the domain of Parallel Computing.

The outcomes/results of the Project can be summarized as follows:

*	A parallel relational database supporting a subset of the typical CRUD (Create, Read, Update, Delete) operations has been designed and implemented.

*	Various CRUD queries fundamental to the functionality of a Database have been implemented as below: 
    *	Creating a Table
    *	Setting a Primary Key
    *	Inserting rows into the table
    *	Enforcing the Primary Key Constraint
    *	Updating the rows of the table including conditional updation
    *	Deleting the rows of the table including conditional deletion
    *	Different variants of the Select Query including Selecting all the columns of all the rows, Selecting all the columns of rows meeting a condition, Selecting a subset of the columns of all rows,  Selecting a subset of the columns of rows meeting a condition.

*	Various aspects related to Parallelism have been identified and implemented as follows:

    *	Select Search Query based on equality of primary key field: Here, a B-Tree has been implemented for indexing the table based on the primary key field. It was observed that the Select Search Query using the B-Tree implementation performed approximately 20x faster than the corresponding sequential implementation.
    
    *	Select Range Query based on primary key field: Here, a B-Tree has been implemented for indexing the table based on the primary key field. It was observed that the Select Range Query using the B-Tree implementation performed approximately 2x faster than the corresponding sequential implementation.
    
    *	Select all Columns for rows meeting a condition: A parallel for loop was used to implement this query. It was observed that the speedup was suboptimal (below 1) in this case with the speedup decreasing with the increase in the number of threads. The main reason is that as this query requires displaying of the rows, the implementation uses a lock-based data structure to serialize the prints. As these overheads due to lock-contention are significant, there are no benefits due to parallelism for this query.
    
    *	Select a subset of the Columns for rows meeting a condition: A parallel for loop was used to implement this query. It was observed that the speedup was suboptimal (below 1) in this case with the speedup decreasing with the increase in the number of threads. The main reason is that as this query requires displaying of the rows, the implementation uses a lock-based data structure to serialize the prints. As these overheads due to lock-contention are significant, there are no benefits due to parallelism for this query.
    
    *	Order by Ascending Query: A Parallel merge sort was used to implement this query. It was observed that the Parallel version achieved a maximum speedup of 2.2 for 16 threads, thereby denoting that there were some benefits due to parallelism.
    
    *	Order by Descending Query: A Parallel merge sort was used to implement this query. It was observed that the Parallel version achieved a maximum speedup of 1.36 for 8 threads, thereby denoting that there were some benefits due to parallelism.
    
    *	Group by Count Query: Here, two different parallel version were implemented:
    
        *	Parallel Version 1: This initially performs a parallel merge sort followed by sequential traversal of the sorted rows to aggregate the count of the groups. This version achieves a maximum speedup of 6 using 32 threads as compared to the sequential version.
        
        *	Parallel Version 2: This initially performs a parallel merge sort followed by a parallel algorithm to aggregate the count of the groups using parallel primitives like tabulate and filter. This version achieves a maximum speedup of 5.5 using 32 threads as compared to the sequential version.
    
        The Parallel Version 1 performs slightly better than the Parallel Version 2. This could be because the overheads introduced by version 2’s algorithm which requires the creation of additional data structures using tabulate and filter is quite considerable that it is outweighing any benefits.
    
    *	Group by Min Query: Here, two different parallel version were implemented:
    
        *	Parallel Version 1: This initially performs a parallel merge sort followed by sequential traversal of the sorted rows to aggregate the min of the groups. This version achieves a maximum speedup of 5.3 using 32 threads as compared to the sequential version.
        
        *	Parallel Version 2: This initially performs a parallel merge sort followed by a parallel algorithm to aggregate the min of the groups using parallel primitives like tabulate, filter and reduce. This version achieves a maximum speedup of 6.5 using 32 threads as compared to the sequential version.
        
        The Parallel Version 2 performs better than the Parallel Version 1. This could be because the parallel version 2 algorithm using primitives like tabulate, filter and reduce inherently contains more scope for parallelism as compared to parallel version 1 which is essentially sequential after the initial parallel sort.
    
    *	Group by Max Query: Here, two different parallel version were implemented:
    
        *	Parallel Version 1: This initially performs a parallel merge sort followed by sequential traversal of the sorted rows to aggregate the max of the groups. This version achieves a maximum speedup of 5.7 using 32 threads as compared to the sequential version.
        
        *	Parallel Version 2: This initially performs a parallel merge sort followed by a parallel algorithm to aggregate the max of the groups using parallel primitives like tabulate, filter and reduce. This version achieves a maximum speedup of 8.1 using 32 threads as compared to the sequential version.
        
        The Parallel Version 2 performs better than the Parallel Version 1. This could be because the parallel version 2 algorithm using primitives like tabulate, filter and reduce inherently contains more scope for parallelism as compared to parallel version 1 which is essentially sequential after the initial parallel sort.
    
    *	Group by Sum Query: Here, two different parallel version were implemented:
    
        *	Parallel Version 1: This initially performs a parallel merge sort followed by sequential traversal of the sorted rows to aggregate the sum of the groups. This version achieves a maximum speedup of 6 using 32 threads as compared to the sequential version.
        
        *	Parallel Version 2: This initially performs a parallel merge sort followed by a parallel algorithm to aggregate the sum of the groups using parallel primitives like tabulate, filter and reduce. This version achieves a maximum speedup of 8.1 using 32 threads as compared to the sequential version.
        
        The Parallel Version 2 performs better than the Parallel Version 1. This could be because the parallel version 2 algorithm using primitives like tabulate, filter and reduce inherently contains more scope for parallelism as compared to parallel version 1 which is essentially sequential after the initial parallel sort.
    
    *	Group by Average Query: Here, two different parallel version were implemented:
    
        *	Parallel Version 1: This initially performs a parallel merge sort followed by sequential traversal of the sorted rows to aggregate the average of the groups. This version achieves a maximum speedup of 5.5 using 32 threads as compared to the sequential version.
        
        *	Parallel Version 2: This initially performs a parallel merge sort followed by a parallel algorithm to aggregate the average of the groups using parallel primitives like tabulate, filter and reduce. This version achieves a maximum speedup of 8.3 using 32 threads as compared to the sequential version.
        
        The Parallel Version 2 performs better than the Parallel Version 1. This could be because the parallel version 2 algorithm using primitives like tabulate, filter and reduce inherently contains more scope for parallelism as compared to parallel version 1 which is essentially sequential after the initial parallel sort.

## How to Run

* The implementation of the parallel relational database is provided as a header file that can just be included in the client to avail the different functionalities. This header file, named _"header_par_db.h"_ is placed inside the ```./include``` directory
* The ```./include``` directory also contains [parlaylib](https://github.com/cmuparlay/parlaylib), which is used to avail the parallel fork-join interface in C++
* Functional Testing
    * Functional Testing can be performed by running the script, ```./run_script.sh``` within the ```./functional_testing``` directory
    * If the _diff_ output from the script shows that the generated sequential and parallel outputs were identical, it denotes that the Functional Testing was successful
    * The recorded outputs are contained in the generated ```./functional_testing/output``` directory
    * It has to be noted that Functional Testing uses a slightly different version of the _"header_par_db.h"_, named _"header_par_db_ft.h"_ contained in the ```./functional_testing``` directory. This is because the parallel sort in _"header_par_db_ft.h"_ switches to a sequential merge and sequential sort for smaller block sizes as compared to the header in ```./include```, which is optimized for performance. This ensures that parallel behaviour is invoked and verified despite the functional tests having fewer records.
* Performance Testing
    * Performance Testing can be performed by running the script, ```./run_script.sh``` within the ```./performance_testing``` directory
    * The recorded timing measurements are contained in the generated ```./performance_testing/timing_measurements``` directory
* *generate_insert.py* inside ```./generate_data``` directory can be used to generate input data
* The ```./generate_graphs``` directory contains the code to generate graphs
* The generated graphs are contained in the ```./generate_graphs/graphs``` directory

