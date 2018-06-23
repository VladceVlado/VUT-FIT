// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: server.h

#include <stdio.h>

#define WAIT_SIZE      10
#define SIZE_OF_LINE   512
#define SIZE_OF_WORD   256
#define SIZE_OF_BUFFER 1024

typedef struct {
	int port;
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
 * @param[in]   argc  The number of arguments
 * @param[in]   argv  The array of arguments
 * @param[out]  cfg   The configuration
 *
 * @return      int   0 everything ok
 *				 	  1 invalid number of arguments
 *				 	  2 invalid argument '-p'
 *				 	  3 invalid format of port (not a number)
 */
int test_args(int argc, char *argv[], Config *cfg);

/**
 * @brief      Signal handler
 *
 * @param[in]  int  sig_num  The signal number to be catched
 */
void sig_handler(int sig_num);

/**
 * @brief       Reads a home directory in /etc/passwd
 *
 * @param[in]   FILE*  fr        Open file /etc/passwd
 * @param[in]   char*  msg_recv  The received message (login)
 * @param[out]  char*  msg_send  The message to be send (home dir)
 */
void read_folder(FILE *fr, char *msg_recv, char *msg_send);

/**
 * @brief       Reads a full name in /etc/passwd
 *
 * @param[in]   FILE*  fr        Open file /etc/passwd
 * @param[in]   char*  msg_recv  The received message (login)
 * @param[out]  char*  msg_send  The message to be send (full name)
 */
void read_name(FILE *fr, char *msg_recv, char *msg_send);

/**
 * @brief       Reads login/logins in /etc/passwd
 *
 * @param[in]   FILE*  fr        Open file /etc/passwd
 * @param[in]   char*  msg_recv  The received message (prefix for login)
 * @param[out]  char*  msg_send  The message to be send (login/logins)
 */
void read_login(FILE *fr, char *msg_recv, char *msg_send);
