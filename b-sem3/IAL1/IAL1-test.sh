#!/bin/bash

# Author:      Vladimir Dusek, xdusek27
# Project:     IAL1 - stack, infix to postfix, double linked list
# University:  VUT FIT 2BIT
# Date:        26/10/2017
# Note:        Advanced testy jsou prevzate ze studentskeho fora fituska.eu,
#              rozhodne nejsem jejich autorem.
# Description: $ chmod +x IAL1-test.sh
#              $ ./IAL1-test.sh


# Test outputs
function test_diff()
{
	DIFF=$(diff "$1" "$2")
	if [ $? -eq 0 ] ; then
		echo -e "\033[0;32m[OK]\033[0m $1 and $2 are identical"
	else
		echo -e "\033[0;31m[ERROR]\033[0m $1 and $2 are not identical"
	fi
}

# Make test directory
DIR_NAME="test-files"
if [ ! -d "${DIR_NAME}" ] ; then
	mkdir "${DIR_NAME}"
fi

# Compiling cycle
for var in c202 c204 c206
do
	echo "Compiling ${var}:"
	standard_test="${var}-test"

	# for c204 there's no advanced test
	if [ "${var}" = "c204" ] ; then
		cd "${var}"
		rm "${standard_test}" 2>/dev/null >/dev/null
		make
		chmod +x "${standard_test}"

   		# STANDARD TEST
    	valgrind --log-file="${standard_test}.valgrind" ./${standard_test} > "${standard_test}.myoutput"
		mv "${standard_test}.myoutput" ../${DIR_NAME}/
		mv "${standard_test}.valgrind" ../${DIR_NAME}/
		cp "${standard_test}.output" ../${DIR_NAME}/

		make clean 2>/dev/null >/dev/null
		cd ..
		echo
		continue
	fi

	advanced_test="${var}-advanced-test"
	cd "${var}"
	rm "${standard_test}" "${advanced_test}" 2>/dev/null >/dev/null 
	make
	chmod +x "${standard_test}"
    chmod +x "${advanced_test}"

    # STANDARD TEST
    valgrind --log-file="${standard_test}.valgrind" ./${standard_test} > "${standard_test}.myoutput"
	mv "${standard_test}.myoutput" ../${DIR_NAME}/
	mv "${standard_test}.valgrind" ../${DIR_NAME}/
	cp "${standard_test}.output" ../${DIR_NAME}/

	# ADVANCED TEST
	valgrind --log-file="${advanced_test}.valgrind" ./${advanced_test} > "${advanced_test}.myoutput"
	mv "${advanced_test}.myoutput" ../${DIR_NAME}/
	mv "${advanced_test}.valgrind" ../${DIR_NAME}/
	cp "${advanced_test}.output" ../${DIR_NAME}/

	make clean 2>/dev/null >/dev/null
	cd ..
	echo
done

cd "${DIR_NAME}"

# Testing cycle
for var in c202 c204 c206
do
	echo "Tests for ${var}:"
	standard_test="${var}-test"	

	echo -n "    standard test: "
	test_diff "${standard_test}.myoutput" "${standard_test}.output"
	echo "    valgrind out:"
	echo "     -----------------------------------------------------------------"
    cat "${standard_test}.valgrind" | sed 's/^.*[=]/     |/g' | grep bytes
    echo -n "     |   "
    echo `cat "${standard_test}.valgrind" | grep "ERROR SUMMARY:" | cut -d ' ' -f 2,3,4,5,6,7,8`
    echo "     -----------------------------------------------------------------"

    # for c204 there's no advanced test
	if [ "${var}" = "c204" ] ; then
		echo
		continue
	fi

    advanced_test="${var}-advanced-test"
	echo -n "    advanced test: "
	test_diff "${advanced_test}.myoutput" "${advanced_test}.output"
	echo "    valgrind out:"
	echo "     -----------------------------------------------------------------"
    cat "${advanced_test}.valgrind" | sed 's/^.*[=]/     |/g' | grep bytes
    echo -n "     |   "
    echo `cat "${advanced_test}.valgrind" | grep "ERROR SUMMARY:" | cut -d ' ' -f 2,3,4,5,6,7,8`
    echo "     -----------------------------------------------------------------"

	echo
done

cd ..

# Remove test files
echo "Do you want to remove test files (y/N)?"
read input
if [ "${input}" = "Y" -o "${input}" = "y" ] ; then
	rm -rf "${DIR_NAME}"
fi
