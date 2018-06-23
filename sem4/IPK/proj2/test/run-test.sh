#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

CNT_RC=0
CNT_OUT=0

function test_return_code()
{
	if [ $1 -eq $2 ] ; then
		echo -e "${GREEN}[OK] ${BLUE}return: ${NC}$1" 
		echo -e "   ${BLUE}expected: ${NC}$2"
	else
		echo -e "${RED}[ERROR] ${BLUE}return: ${NC}$1"
		echo -e "      ${BLUE}expected: ${NC}$2"
		((CNT_RC++))
	fi
}

function test_output()
{
	if [ "${1}" == "${2}" ] ; then
		echo -e "${GREEN}[OK] ${BLUE}output: ${NC}${1}"
		echo -e "   ${BLUE}expected: ${NC}${2}"
	else
		echo -e "${RED}[ERROR] ${BLUE}output: ${NC}${1}"
		echo -e "      ${BLUE}expected: ${NC}${2}"
		((CNT_OUT++))
	fi
}

echo "-------------------------------------------------------"
for folder in `ls -d */` ; do
	cd $folder
	echo -e "${BLUE}test: ${NC}test/$folder"
	echo -e -n "${BLUE}exe: ${NC}"
	cat exe | cut -c 11-
	echo
	out=$(eval `cat exe`)
	test_return_code "$?" `cat rc`
	test_output "$out" "`cat out`"
	echo "-------------------------------------------------------"
	cd ..	
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
