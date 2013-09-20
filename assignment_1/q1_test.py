#!/usr/bin/python

from subprocess import check_output

def run_for_seed(command, seed, times):
    cmd = [command, str(seed), str(times)]
    print "running: ", cmd
    return check_output(cmd)

def run_for_seed_range(commands, seeds_start, seeds_end):
    for seed in xrange(seeds_start, seeds_end):
        # get all of the output
        reference_output = None
        for command in commands:
            output = str(run_for_seed(command, seed, 1000))
            if reference_output is None:
                reference_output = output
            else:
                if reference_output == output:
                    continue
                print "Error command: {}, seed: {} got:\n{}should be:\n{}" \
                    .format(command, seed, output, reference_output)
                return
        print seed, " passed"

def main():
    run_for_seed_range(
        ["./exception", "./flags"],
        0,
        1000)

if __name__ == "__main__":
    main()
