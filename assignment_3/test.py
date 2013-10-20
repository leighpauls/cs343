#!/usr/bin/env python

import subprocess

def run_cmd(command, num_reducers, queue_length, sort_buffer_size):
    cmd = [command,
           "test_words",
           str(num_reducers),
           str(queue_length),
           str(sort_buffer_size)]
    output = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE).communicate()[0]
    # print "output is: ", output, " from ", cmd
    return output

def word_counts_from(word_count_string):
    res = {}
    # print "reading from ", word_count_string
    for line in word_count_string.split("\n")[:-2]:
        # print "reading ", line
        word, count = line.split(" : ")
        res[word] = int(count)
    return res

def compare(num_reducers, queue_length, sort_buffer_size):
    good_raw_output = run_cmd(
        "./wordcount_64",
        num_reducers,
        queue_length,
        sort_buffer_size)
    test_raw_output = run_cmd(
        "./wordcount",
        num_reducers,
        queue_length,
        sort_buffer_size)
    good_word_count = word_counts_from(good_raw_output)
    test_word_count = word_counts_from(test_raw_output)
    if good_word_count == test_word_count:
        return True
    else:
        print num_reducers, ',', queue_length, ',', sort_buffer_size, ' failed!'
        print "good:---------------------------"
        print good_word_count
        print good_raw_output
        print "bad:----------------------------"
        print test_word_count
        print test_raw_output
        return False

def test_range():
    for num_reducers in xrange(1, 10):
        for queue_len in xrange(1, 10):
            for sort_buffer_size in xrange(0, 10):
                if not compare(num_reducers, queue_len, sort_buffer_size):
                    exit(1)
    print "All tests passed!"

if __name__ == "__main__":
    test_range()
