# !/bin/bash
#
# training.sh -- training file for player using pre-compiled dealer
#
# usage: bash -v training.sh 
# run this before running training.sh in player directory in separate terminal
#
# input: 
# output: test runs and results
#
# Alex Fick, CS50, Fall 2022

for i in {1..10000}
do
    ./dealer-given 1 100 8099
done

