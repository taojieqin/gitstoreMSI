#!/bin/bash

echo "test use 1"

test -w tmp.txt

echo $?

echo "test use 2 [] begin"

[ -w tmp.txt ]

echo $?
