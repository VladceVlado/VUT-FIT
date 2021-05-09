#!/usr/bin/env bash

# VUT FIT MITAI
# KRY 2020/2021
# Project 1: Vigenere cipher
# Author: Vladimir Dusek
# Login: xdusek27

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

CNT_RC=0
CNT_OUT=0
CNT_TIMEOUT=0

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
    if [[ $(echo "$1" | cut -d ';' -f 1 --complement) == $(echo "$2" | cut -d ';' -f 1 --complement) ]]; then
        echo -e "${GREEN}[OK] ${BLUE}output:${NC}\n${1}"
        echo -e "   ${BLUE}expected:${NC}\n${2}"
    else
        echo -e "${RED}[ERROR] ${BLUE}output:${NC}\n${1}"
        echo -e "      ${BLUE}expected:${NC}\n${2}"
        ((CNT_OUT++))
    fi
}

function test_timeout() {
    if (($(echo "$1 <= 30" | bc -l))); then
        echo -e "${GREEN}[OK] ${BLUE}execution time:${NC} ${1}s"
    else
        echo -e "${RED}[ERROR] ${BLUE}execution time:${NC} ${1}s"
        ((CNT_TIMEOUT++))
    fi
}

echo "-------------------------------------------------------"

echo -e "${BLUE}compiling:${NC}"

cd src/ && make && cd ../
RC=$?

if [[ $RC -ne 0 ]]; then
    echo -e "${RED}[ERROR]${NC}: compilation was not successful"
    exit
else
    echo -e "${GREEN}[OK]${NC}: compilation was successful"
fi

echo "======================================================="

for DIR in test/*; do

    echo -e "${BLUE}test: ${NC}${DIR}"
    cat "${DIR}/readme"

    echo "-------------------------------------------------------"

    RUN="./src/kry < ${DIR}/ciphertext"
    echo -e "${BLUE}run: ${NC}${RUN}"

    START_TIME=$(date +%s.%3N)
    OUT=$(eval "${RUN}")
    RC=$?
    END_TIME=$(date +%s.%3N)

    test_timeout "$(echo "$END_TIME - $START_TIME" | bc -l)"
    test_return_code "$RC" "0"
    test_output "$OUT" "$(cat "$DIR"/out)"

    echo "======================================================="
done

echo -e "${BLUE}cleaning:${NC}"

cd src/ && make clean && cd ../
RC=$?

if [[ $RC -ne 0 ]]; then
    echo -e "${RED}[ERROR]${NC}: cleaning was not successful"
    exit
else
    echo -e "${GREEN}[OK]${NC}: cleaning was successful"
fi

echo "-------------------------------------------------------"

echo -e "${BLUE}SUMMARY:${NC}"

if [[ "$CNT_TIMEOUT" -eq 0 ]]; then
    echo -e "${GREEN}timeouts: $CNT_TIMEOUT"
else
    echo -e "${RED}timeouts: $CNT_TIMEOUT"
fi

if [[ "$CNT_RC" -eq 0 ]]; then
    echo -e "${GREEN}wrong return codes: $CNT_RC"
else
    echo -e "${RED}wrong return codes: $CNT_RC"
fi

if [[ "$CNT_OUT" -eq 0 ]]; then
    echo -e "${GREEN}wrong outputs: $CNT_OUT"
else
    echo -e "${RED}wrong outputs: $CNT_OUT"
fi

echo -e "${NC}-------------------------------------------------------"
