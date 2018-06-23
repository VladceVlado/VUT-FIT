// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: client.h

#define _BSD_SOURCE
#define SIZE 1024

typedef enum {
	N,
	F,
	L
} Arg3;

typedef struct {
	char *host;
	int port;
	char *login;
	Arg3 arg3;
} Config;

/**
 * @brief      Create new config and initialize it
 *
 * @return     Config*  Pointer to the allocated and initialized configuration
 */
Config *new_cfg_init();

/**
 * @brief      Destroy config, free all memory and set all pointers to NULL
 *
 * @param[in]  Config*  cfg  The configuration to be freed
 */
void destroy_cfg(Config *cfg);

/**
 * @brief       Test the correctness of command line arguments
 *
 * @param[in]   int      argc  The number of arguments
 * @param[in]   char*[]  argv  The array of arguments
 * @param[out]  Config*  cfg   The configuration
 *
 * @return      int   0 everything ok
 *				 	  1 invalid number of arguments
 *				 	  2 invalid argument
 *				 	  3 multiple times same argument
 *				 	  4 login is too long (> 1024B)
 */
int test_args(int argc, char *argv[], Config *cfg);

/**
 * @brief       Convert host/domain name to ip address,
 * 			    source: https://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
 *
 * @param[in]   char*  host  The host/domain name to be convert
 * @param[out]  char*  ip    Converted ip address
 *
 * @return      int    0 if everything ok
 * 					   non-zero if error
 */
int host_to_ip(char *host, char *ip);

/**
 * @brief       Convert data from config to a message for server
 *
 * @param[in]   Config*  cfg       The configuration
 * @param[out]  char*    msg_send  The message send
 */
void convert_msg(Config *cfg, char *msg);
