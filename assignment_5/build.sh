#!/bin/sh

set -e

cd ~/class/cs343/assignment_5/
make phil TYPE=AUTO
./phil 5 6 1

make phil TYPE=INTB
./phil 5 6 1

make phil TYPE=INT
./phil 5 6 1


