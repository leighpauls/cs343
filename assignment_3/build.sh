#!/bin/sh
set -e

cd ~/class/cs343/assignment_3/
make wordcount

./wordcount test_words
