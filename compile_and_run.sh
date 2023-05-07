#!/bin/bash

make dist/$1 &&

echo -e "Running $1...\n" &&

"./dist/$1" $2 $3 $4 $5
