#!/usr/bin/env python
# Autotest.py
# tests all of the boarder of valid north pole test cases, ensuring that there
# are no errors, most importantly it checks both above and below the the cutoff
# numbers of elves and consultations required to make santa actually do
# consultations.
# Failure cases ususally end up ina deadlock, so if this script completes, then
# the program is probably valid.

import subprocess

MIN_ARG_VALUES = [1, 1, 1, 0, 0]
MAX_ARG_VALUES = [10, 10, 3, 10, 10]

def run_cmd(argValues):
    # print "running for: ", argValues
    stringValues = [str(x) for x in argValues]
    cmd = ['./northpole'] + stringValues
    output = subprocess.check_output(cmd)
    # print output

def try_all_for_num_args(numArgs):
    """try all the argument combos within the range specified"""
    startArgs = MIN_ARG_VALUES[0:numArgs]
    curArgs = startArgs[:]
    endArgs = MAX_ARG_VALUES[0:numArgs]

    def incrementArgs(pos):
        """return True if it can increment curArgs and not pass endArgs"""
        if pos < -len(curArgs):
            return False
        curArgs[pos] += 1
        if curArgs[pos] <= endArgs[pos]:
            return True
        curArgs[pos] = startArgs[pos]
        return incrementArgs(pos-1)

    while True:
        run_cmd(curArgs)
        if not incrementArgs(-1):
            break

def try_all_inputs():
    for numArgs in xrange(len(MIN_ARG_VALUES)):
        try_all_for_num_args(numArgs)
    print "Success!"

if __name__ == "__main__":
    try_all_inputs()
