#!/bin/sh

make resumption noresumption
echo "RESUMPTION:"
./resumption 25 0
echo "NORESUMPTION:"
./noresumption 25 0

