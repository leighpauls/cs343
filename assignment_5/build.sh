#!/bin/sh

set -e

cd ~/class/cs343/assignment_5/
make phil TYPE=INT
./phil

make phil TYPE=INTB
./phil