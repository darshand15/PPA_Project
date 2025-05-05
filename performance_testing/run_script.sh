#!/bin/bash

rm -rf timing_measurements
mkdir timing_measurements

g++ -g -std=c++17 -I../include/ -pthread perf_testing_10K.cpp -o par_db

export PARLAY_NUM_THREADS=1
./par_db > /dev/null

export PARLAY_NUM_THREADS=2
./par_db > /dev/null

export PARLAY_NUM_THREADS=4
./par_db > /dev/null

export PARLAY_NUM_THREADS=8
./par_db > /dev/null

export PARLAY_NUM_THREADS=16
./par_db > /dev/null

export PARLAY_NUM_THREADS=32
./par_db > /dev/null
