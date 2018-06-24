// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: error.c

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void call_error(Err_type err_type)
{
	switch (err_type) {
		case ERR_NO_MATCH:
			fprintf(stderr, "%s\n", "error: no match for login");
			exit(RET_NO_MATCH);

		case ERR_ARGS:
			fprintf(stderr, "%s\n", "error: invalid command line options");
			exit(RET_ARGS);

		case ERR_FOPEN:
			fprintf(stderr, "%s\n", "error: opening /etc/passwd");
			exit(RET_FOPEN);

		case ERR_HOST:
			fprintf(stderr, "%s\n", "error: cannot convert hostname to ip");
			exit(RET_HOST);

		case ERR_SOCK:
			fprintf(stderr, "%s\n", "error: socket");
			exit(RET_SOCK);

		case ERR_BIND:
			fprintf(stderr, "%s\n", "error: bind");
			exit(RET_BIND);

		case ERR_CONN:
			fprintf(stderr, "%s\n", "error: connect");
			exit(RET_CONN);

		case ERR_LIST:
			fprintf(stderr, "%s\n", "error: listen");
			exit(RET_LIST);

		case ERR_ACCE:
			fprintf(stderr, "%s\n", "error: accept");
			exit(RET_ACCE);

		case ERR_SEND:
			fprintf(stderr, "%s\n", "error: send");
			exit(RET_SEND);

		case ERR_RECV:
			fprintf(stderr, "%s\n", "error: reveive");
			exit(RET_RECV);

		case ERR_ALLOC:
			fprintf(stderr, "%s\n", "error: memory allocation failed");
			exit(RET_ALLOC);
	}
}
