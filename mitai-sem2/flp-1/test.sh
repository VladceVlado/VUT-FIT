#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'  # No Color

CNT_RC=0
CNT_OUT=0

function test_return_code()
{
    if [ "$1" -eq "$2" ] ; then
        echo -e "${GREEN}[OK] ${BLUE}return: ${NC}$1"
#        echo -e "   ${BLUE}expected: ${NC}$2"
    else
        echo -e "${RED}[ERROR] ${BLUE}return: ${NC}$1"
        echo -e "      ${BLUE}expected: ${NC}$2"
        ((CNT_RC++))
    fi
}

function test_output()
{
    if [ "$1" == "$2" ] ; then
        echo -e "${GREEN}[OK] ${BLUE}output${NC}"
#        echo -e "   ${BLUE}expected: \n${NC}${2}"
    else
        echo -e "${RED}[ERROR] ${BLUE}output: \n${NC}${1}"
        echo -e "      ${BLUE}expected: \n${NC}${2}"
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

echo "=========================================================="

for DIR in test/* ; do

    echo -e "${BLUE}test: ${NC}${DIR}"

    echo "-------------------------------------------------------"

    RUN="./plg-2-nka -i ${DIR}/in 2> /dev/null"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$(cat "$DIR"/rc)" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/in)"
    fi

    echo "-------------------------------------------------------"

    RUN="cat ${DIR}/in | ./plg-2-nka -i 2> /dev/null"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$(cat "$DIR"/rc)" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/in)"
    fi

    echo "-------------------------------------------------------"

    RUN="./plg-2-nka -1 ${DIR}/in 2> /dev/null"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$(cat "$DIR"/rc)" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/1)"
    fi

    echo "-------------------------------------------------------"

    RUN="cat ${DIR}/in | ./plg-2-nka -1 2> /dev/null"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$(cat "$DIR"/rc)" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/1)"
    fi

    echo "-------------------------------------------------------"

    RUN="./plg-2-nka -2 ${DIR}/in"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$RC" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/2)"
    fi

    echo "-------------------------------------------------------"

    RUN="cat ${DIR}/in | ./plg-2-nka -2 2> /dev/null"
    echo -e "${BLUE}run: ${NC}${RUN}"
    OUT=$(eval "${RUN}")
    RC=$?

    test_return_code "$RC" "$(cat "$DIR"/rc)"

    if [ "$RC" == 0 ] ; then
        test_output "$OUT" "$(cat "$DIR"/2)"
    fi

    echo "=========================================================="
done

echo -e "${BLUE}SUMMARY:${NC}"
if [ $CNT_RC -eq 0 ] ; then
    echo -e "${GREEN}wrong return codes =" $CNT_RC
else
    echo -e "${RED}wrong return codes =" $CNT_RC
fi

if [ $CNT_OUT -eq 0 ] ; then
    echo -e "${GREEN}wrong outputs =" $CNT_OUT
else
    echo -e "${RED}wrong outputs =" $CNT_OUT
fi

echo -e "${NC}-------------------------------------------------------"
