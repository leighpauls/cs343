#!/usr/bin/env python

import subprocess, random

def generate_words(num_files, max_words_per_file, num_words):
    word_bag = []
    for i in xrange(num_words):
        word = subprocess.Popen(
            ["./random_word.sh"],
            shell=True,
            stdout=subprocess.PIPE).communicate()[0]
        word = word.split("\n")[0]
        word_bag.append(word)

    for i in xrange(num_files):
        with open("random_words/gen_words" + str(i) + ".txt", 'w') as f:
            for _ in xrange(random.randrange(max_words_per_file)):
                f.write(random.choice(word_bag) + "\n")

if __name__ == "__main__":
    generate_words(10, 100, 300)
