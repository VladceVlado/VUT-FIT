#!/usr/bin/env bash

# VUT FIT MITAI
# PDS 2021/2022
# Project: Identification of Mobile Traffic using TLS Fingerprinting
# Author: Vladimir Dusek
# Login: xdusek27

# Exit immediately if a command fails
set -o errexit

# Exit immediately if an undefined variable is used
set -o nounset

# Exit immediately if a command in a pipe fails
set -o pipefail

python3 src/train.py
