#!/bin/bash

for i in {2..25} ; do
    echo "$i" >> out
    for _ in {1..29} ; do
        ./test.sh $i 2>> out
    done
    echo >> out
done
