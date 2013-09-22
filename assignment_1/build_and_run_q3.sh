#!/bin/sh

make utf8
g++ utf8.cc -o utf8_gen
echo "MINE:"
./utf8_gen | ./utf8
echo ""
echo "THEIRS:"
./utf8_gen | ./utf864

