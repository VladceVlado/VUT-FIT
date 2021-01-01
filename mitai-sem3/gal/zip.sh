#!/usr/bin/env bash

mkdir odevzdat/

cp docs/docs.pdf odevzdat/docs.pdf
cp slides/slides.pdf odevzdat/slides.pdf

mkdir odevzdat/src/
cp src/* odevzdat/src/

mkdir odevzdat/scripts/
cp scripts/* odevzdat/scripts/

cp run.sh odevzdat/run.sh

cd odevzdat/

zip -r ../xdusek27_xgolds00.zip *

cd ../

rm -rf odevzdat/
