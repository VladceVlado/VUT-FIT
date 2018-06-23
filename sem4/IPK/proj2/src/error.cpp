// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: error.cpp

#include "error.h"
#include <iostream>

using namespace std;

void error(Err_type err_type)
{
	switch (err_type) {
		case ERR_TIMEOUT:
			cerr << "error: timeout" << endl;
			exit(RET_NOT_FOUND);

		case ERR_INV_OPTS:
			cerr << "error: invalid command line options" << endl;
			cerr << "try \"./ipk-lookup -h\" for more information" << endl;
			exit(RET_INV_OPTS);

		case ERR_INV_TIMEOUT:
			cerr << "error: invalid parameter -T <timeout>" << endl;
			cerr << "<timeout> has to be integer (5 default)" << endl;
			exit(RET_INV_OPTS);

		case ERR_INV_TYPE:
			cerr << "error: invalid parameter -t <type>" << endl;
			cerr << "<type> e {A (default), AAAA, NS, PTR, CNAME}" << endl;
			exit(RET_INV_OPTS);

		case ERR_INV_SERVER:
			cerr << "error: invalid parameter -s <server>" << endl;
			cerr << "<server> has to be ipv4" << endl;
			exit(RET_INV_OPTS);

		case ERR_INV_PTR:
			cerr << "error: invalid value of -t PTR" << endl;
			cerr << "PTR has to be ipv4 or ipv6 address" << endl;
			exit(RET_INV_OPTS);

		case ERR_SYS_SOCKET:
			cerr << "error: socket() failed" << endl;
			exit(RET_SYS);

		case ERR_SYS_SETSOCKET:
			cerr << "error: setsockopt() failed" << endl;
			exit(RET_SYS);

		case ERR_SYS_SENDTO:
			cerr << "error: sendto() failed" << endl;
			exit(RET_SYS);

		case ERR_SYS_ALLOC:
			cerr << "error: malloc() failed" << endl;
			exit(RET_SYS);

		case ERR_UNKNOWN:
			cerr << "unknown error" << endl;
			exit(RET_UNKNOWN);
	}
}
