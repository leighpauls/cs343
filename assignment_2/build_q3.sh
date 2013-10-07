#!/bin/sh
set -e

cd ~/class/cs343/assignment_2/
make mergesort

./mergesort values.in
./mergesort_64 values.in

echo "\n\nMine for 1:"
time -p ./mergesort size.in -1
echo "\n\nTheirs for 1:"
time -p ./mergesort_64  size.in -1

echo "\n\nMine for 2:"
time -p ./mergesort size.in -2
echo "\n\nTheirs for 2:"
time -p ./mergesort_64  size.in -2
