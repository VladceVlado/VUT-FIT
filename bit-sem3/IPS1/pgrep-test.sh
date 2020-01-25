#!/bin/bash

# Author:      Vladimir Dusek, xdusek27
# Project:     IPS - proj1, parallel grep
# University:  VUT FIT 2BIT
# Date:        13/10/2017
# Description: $ chmod +x pgrep-test.sh
#              $ ./pgrep-test.sh


function test_diff()
{
	my_out=$(eval "./src/pgrep $1")
	sample_out=$(eval "./test/pgrep_sample $1")
	diff <(echo "$my_out") <(echo "$sample_out")
	if [ $? -eq 0 ] ; then
		echo -e "\033[0;32m[OK]\033[0m ./pgrep $1"
	else
		echo -e "\033[0;31m[ERROR]\033[0m ./pgrep $1"
	fi
}

if [ ! -f src/pgrep ] ; then
	cd src/
	make
	cd ../
fi

test_diff '1 ".*wheel.*" 1 < test/sudoers'
test_diff '0 ".*wheel.*" -1 < test/sudoers'
test_diff '1 ".*wheel.*" -1 ".* ALL.*" 1 < test/sudoers'
test_diff '1 ".*wheel.*" -1 ".* ALL.*" 1 ".*cd.*" 1 < test/sudoers'
test_diff '2 ".*wheel.*" -1 ".* ALL.*" 1 ".*cd.*" 1 < test/sudoers'
test_diff '2 ".*wheel.*" -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < test/sudoers'
test_diff '3 ".*wheel.*" -1 ".* ALL.*" 1 ".*cd.*" 1 ".*root.*" 1  < test/sudoers'
test_diff '3 ".*wheel.*" 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  < test/sudoers'
test_diff '3 ".*wheel.*" 1 ".* ALL.*" 2 ".*cd.*" 1 ".*root.*" 1  ".*NOPASSWD.*" -5 < test/sudoers'
test_diff '7 ".*NETWORKING,.*" 1 ".*SOFTWARE,.*" 1 ".*SERVICES,.*" 1 ".*STORAGE,.*" 1 ".*DELEGATING,.*" 1 "PROCESSES,.*" 1 ".*LOCATE,.*" 1 ".*DRIVERS.*" 1 < test/sudoers'
test_diff '2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 < test/group'
test_diff '1 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 < test/group'
test_diff '2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 < test/group'
test_diff '2 "[^:]*-[^:]*:.*" 1 ".*:4.*" 1 ".*journal.*" -5 "root.*" 5 < test/group'
test_diff '3 "Ahoj.*" 1 "[0-9][0-9]* .*" 2 "[^0-9][^0-9]* .*" 3 < test/example'
test_diff '2 ".*pgrep.*" 1 ".*pthread" 1 < zadani/zadani.txt'
test_diff '2 ".*pgrep.*" 1 ".*pthread.*" 1 < zadani/zadani.txt'
test_diff '1 ".*pgrep.*" 1 ".*pthread.*" -1 < zadani/zadani.txt'

# Pro jistotu zakomentovat nasledujici radek a pustit v nekonecnem cyklu
# $ while true ; do ./pgrep_test.sh ; done
rm src/pgrep
