#!/usr/bin/env bash

# VUT FIT MITAI
# BMS 2020/2021
# Project: Convolutional coder and Viterbi algorithm
# Author: Vladimir Dusek
# Login: xdusek27

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

CNT_RC=0
CNT_OUT=0

function test_return_code() {
    if [[ "$1" -eq "$2" ]]; then
        echo -e "${GREEN}[OK] ${BLUE}return:${NC} $1"
        echo -e "   ${BLUE}expected:${NC} $2"
    else
        echo -e "${RED}[ERROR] ${BLUE}return:${NC} $1"
        echo -e "      ${BLUE}expected:${NC} $2"
        ((CNT_RC++))
    fi
}

function test_output() {
    if [[ "$1" == "$2" ]]; then
        echo -e "${GREEN}[OK] ${BLUE}output:${NC}\n${1}"
        echo -e "   ${BLUE}expected:${NC}\n${2}"
    else
        echo -e "${RED}[ERROR] ${BLUE}output:${NC}\n${1}"
        echo -e "      ${BLUE}expected:${NC}\n${2}"
        ((CNT_OUT++))
    fi
}

echo "-------------------------------------------------------"

echo -e "${BLUE}compiling:${NC}"

if ! make; then
    echo -e "${RED}[ERROR]${NC}"
    exit
else
    echo -e "${GREEN}[OK]${NC}"
fi

echo "======================================================="

for DIR in test/*; do

    echo -e "${BLUE}test: ${NC}${DIR}"

    echo "-------------------------------------------------------"

    RUN=$(cat "${DIR}/exe")
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    test_output "$OUT" "$(cat "$DIR"/out)"

    echo "======================================================="
done

echo -e "${BLUE}cleaning:${NC}"

if ! make clean; then
    echo -e "${RED}[ERROR]${NC}"
    exit
else
    echo -e "${GREEN}[OK]${NC}"
fi

echo "-------------------------------------------------------"

echo -e "${BLUE}SUMMARY:${NC}"
if [ "$CNT_RC" -eq 0 ]; then
    echo -e "${GREEN}wrong return codes =" "$CNT_RC"
else
    echo -e "${RED}wrong return codes =" "$CNT_RC"
fi

if [ "$CNT_OUT" -eq 0 ]; then
    echo -e "${GREEN}wrong outputs =" "$CNT_OUT"
else
    echo -e "${RED}wrong outputs =" "$CNT_OUT"
fi

echo -e "${NC}-------------------------------------------------------"
