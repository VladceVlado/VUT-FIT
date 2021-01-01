#!/usr/bin/env bash

for DIR in data_graphs/ data_times/ plots/; do
    if [[ -d $DIR ]]; then
        rm -rf $DIR
    fi
done

cd src/ || exit
make clean
make
cd ..

./scripts/generate_graphs.py
./scripts/run_measurement.py
./scripts/get_plots.py
