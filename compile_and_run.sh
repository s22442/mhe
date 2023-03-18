#!/bin/bash

rm -rf dist &&

mkdir "dist" &&

g++ -o "dist/main" "src/$1/main.cpp" &&

"./dist/main"
