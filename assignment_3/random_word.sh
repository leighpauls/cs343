#!/bin/sh

cat -n /usr/share/dict/words | grep -w $( jot -r 1 1 99171 ) | cut -f2