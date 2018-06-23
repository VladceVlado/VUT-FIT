// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: error.h

typedef enum {
	RET_NO_MATCH = 1,
	RET_ARGS = 10,
	RET_FOPEN = 20,
	RET_HOST = 30,
	RET_SOCK = 40,
	RET_BIND = 41,
	RET_CONN = 42,
	RET_LIST = 43,
	RET_ACCE = 44,
	RET_SEND = 45,
	RET_RECV = 46,
	RET_ALLOC = 99
} Ret_code;

typedef enum {
	ERR_NO_MATCH,
	ERR_ARGS,
	ERR_FOPEN,
	ERR_HOST,
	ERR_SOCK,
	ERR_BIND,
	ERR_CONN,
	ERR_LIST,
	ERR_ACCE,
	ERR_SEND,
	ERR_RECV,
	ERR_ALLOC
} Err_type;

/**
 * @brief      Print error message on stderr and exit program
 *
 * @param[in]  err_type  Specify the error
 */
void call_error(Err_type err_type);
