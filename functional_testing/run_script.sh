#!/bin/bash

rm -rf output
mkdir output

export PARLAY_NUM_THREADS=4
g++ -g -std=c++17 -I../include/ -pthread func_test_client_seq.cpp -o seq_db
./seq_db > ./output/seq_out

g++ -g -std=c++17 -I../include/ -pthread func_test_client_par_1.cpp -o par_1_db
./par_1_db > ./output/par_1_out

g++ -g -std=c++17 -I../include/ -pthread func_test_client_par_2.cpp -o par_2_db
./par_2_db > ./output/par_2_out

diff -s -B <(sort ./output/seq_out) <(sort ./output/par_1_out)
diff -s -B <(sort ./output/seq_out) <(sort ./output/par_2_out)
