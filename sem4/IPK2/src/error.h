// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: error.h

#pragma once

/**
 * All types of return codes.
 */
enum Ret_code {
	RET_NOT_FOUND = 1, // non-existent record or timeout
	RET_INV_OPTS = 2,  // invalid command line options
	RET_SYS = 3,       // system error (malloc, socket, etc.)
	RET_UNKNOWN = 99   // unknown error (it should not happen)
};

/**
 * All types of possible errors.
 */
enum Err_type {
	ERR_TIMEOUT,       // recv() failed, timeout
	ERR_INV_OPTS,      // invalid command line arguments
	ERR_INV_TIMEOUT,   // invalid parameter timeout
	ERR_INV_TYPE,      // invalid parameter type
	ERR_INV_SERVER,    // invalid ipv4 server
	ERR_INV_PTR,       // invalid -T PTR, not address
	ERR_SYS_SOCKET,    // socket() failed
	ERR_SYS_SETSOCKET, // setsocket() failed
	ERR_SYS_SENDTO,    // sendto() failed
	ERR_SYS_ALLOC,     // malloc() failed
	ERR_UNKNOWN        // unknown error
};

/**
 * Print error message on stderr and exit the program according
 * to err_type parameter.
 */
void error(Err_type err_type);
