#!/usr/bun/env python

import subprocess
import sys

def run_cmd(command, players, seed):
    cmd = [command, str(players), str(seed)]
    output = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    return output

def compare_for_seed(players, seed):
    good_output = run_cmd("./hotpotato_64", players, seed)
    test_output = run_cmd("./hotpotato", players, seed)
    if good_output == test_output:
        print players, ",  ", seed, " Matches!"
        return True
    else:
        print players, ",  ", seed, " failed!"
        print "good:----------------------------------------"
        print good_output
        print "bad:-----------------------------------------"
        print test_output
        return False

def run_range():
    for seed in xrange(0, 200):
        for players in xrange(2, 21):
            if not compare_for_seed(players, seed):
                return


if __name__ == "__main__":
    run_range()
