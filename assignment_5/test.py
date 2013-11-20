#!/usr/bin/env python
# test.py
# Tests to verify that no valid input can cause any of the program types to
# deadlock. If this program finishs without exception, then it is reasonable to
# assume that the program is correct.
#
# This tests a bunch of permutations possible input values for the program. Most
# importantly, it tests the border cases (those values adjacent to the invalid
# input arguments), and a few larger inputs to better verify the correctness
# of the solution.
#

import subprocess


MIN_ARG_VALUES = [2, 1, 1]
MAX_ARG_VALUES = [10, 10, 10]

# If the process exits successfully, then it did not deadlock
# Therefore the test case passes if this function returns
def run_cmd(argValues):
    stringValues = [str(x) for x in argValues]
    cmd = ['./phil'] + stringValues
    if subprocess.check_call(cmd, stdout=subprocess.PIPE):
        raise Exception(str(argValues) + " Failed!!!")

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

BUILD_TYPES = ['INTB', 'TASK', 'AUTO', 'INT']

def try_all_build_types():
    for build_type in BUILD_TYPES:
        print build_type, "..."
        subprocess.check_call(
            ['make', 'phil', 'TYPE='+build_type],
            stdout=subprocess.PIPE)
        try_all_inputs()
    print "All build types completed!"

if __name__ == "__main__":
    try_all_build_types()

