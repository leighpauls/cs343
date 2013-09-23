#!/bin/sh

make utf8
g++ q3utf8_gen.cc -o utf8_gen
echo "running mine..."
./utf8_gen | ./utf8 > mine.out
cat mine.out
echo "runnig theirs..."
./utf8_gen | ./utf864 > theirs.out
cat theirs.out
echo "errors shown here... (no output means no failures)"
diff -C 2 mine.out theirs.out
rm mine.out theirs.out
