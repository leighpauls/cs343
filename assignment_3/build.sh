#!/bin/bash
set -e

cd ~/class/cs343/assignment_3/
make wordcount

for i in {1..10}
do 
    ./wordcount test_words
done

