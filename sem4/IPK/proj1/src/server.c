// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: server.c

#include "server.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

FILE *fr = NULL;
int new_socket, server_socket_fd;
Config *cfg = NULL;

Config *new_cfg_init()
{
	Config *cfg = calloc(1, sizeof(Config));
	if (cfg == NULL)
		call_error(ERR_ALLOC);
	return cfg;
}

void destroy_cfg(Config *cfg)
{
	if (cfg != NULL) {
		free(cfg);
		cfg = NULL;
	}
}

int test_args(int argc, char *argv[], Config *cfg)
{
	if (argc != 3)
		return 1;
	if (strcmp(argv[1], "-p") != 0)
		return 2;
	for (int i = 0; i < (int) strlen(argv[2]); i++) {
		if (!isdigit(argv[2][i]))
			return 3;
	}
	cfg->port = atoi(argv[2]);
	return 0;
}

void sig_handler(int sig_num)
{
    if (sig_num == SIGINT) {
    	putchar('\n');
		fclose(fr);
		close(server_socket_fd);
		close(new_socket);
		destroy_cfg(cfg);
    	exit(0);
    }
}

void read_folder(FILE *fr, char *msg_recv, char *msg_send)
{
	char line_of_file[SIZE_OF_LINE] = {0};
	char word[SIZE_OF_WORD] = {0};
	int i, j, count_colon = 0;

	while (fgets(line_of_file, SIZE_OF_BUFFER, fr) != NULL) {
        memset(&word, '\0', sizeof(word));
        for (i = 0; line_of_file[i] != ':'; i++) {
				word[i] = line_of_file[i];
		}
		if (strcmp(word, msg_recv) == 0) {
			for (i = 0, j = 0; true; i++) {
				if (count_colon < 5) { // jeste nejsem u home dir
					if (line_of_file[i] == ':')
						count_colon++;
				}
				else {
					if (line_of_file[i] == ':') // uz jsem za home dir
						break;
					msg_send[j++] = line_of_file[i];
				}
			}
		}
    }
    strcat(msg_send, " ");
}

void read_name(FILE *fr, char *msg_recv, char *msg_send)
{
	char line_of_file[SIZE_OF_LINE] = {0};
	char word[SIZE_OF_WORD] = {0};
	int i, j, count_colon = 0;

	while (fgets(line_of_file, SIZE_OF_BUFFER, fr) != NULL) {
        memset(&word, '\0', sizeof(word));
        for (i = 0; line_of_file[i] != ':'; i++) {
				word[i] = line_of_file[i];
		}
		if (strcmp(word, msg_recv) == 0) { // nasel jsem radek s loginem
			for (i = 0, j = 0; true; i++) {
				if (count_colon < 4) { // jeste nejsem u jmena
					if (line_of_file[i] == ':')
						count_colon++;
				}
				else {
					if (line_of_file[i] == ':') // uz jsem za jmenem
						break;
					msg_send[j++] = line_of_file[i];
				}
			}
		}
    }
    strcat(msg_send, " ");
}

void read_login(FILE *fr, char *msg_recv, char *msg_send)
{
	char line_of_file[SIZE_OF_LINE] = {0};
	char word[SIZE_OF_WORD] = {0};

	if (strcmp(msg_recv, "L") == 0) {
		while (fgets(line_of_file, SIZE_OF_LINE, fr) != NULL) {
			memset(&word, '\0', sizeof(word));
			for (int i = 0; line_of_file[i] != ':'; i++) {
				word[i] = line_of_file[i];
			}
			strcat(msg_send, word);
			strcat(msg_send, "\n");
		}
	}
	else {
		while (fgets(line_of_file, SIZE_OF_LINE, fr) != NULL) {
			memset(&word, '\0', sizeof(word));
			for (int i = 0; line_of_file[i] != ':'; i++) {
				word[i] = line_of_file[i];
			}
			if (strncmp(msg_recv + sizeof(char), word,
				strlen(msg_recv + sizeof(char))) != 0) {
				continue;
			}
			strcat(msg_send, word);
			strcat(msg_send, "\n");
		}
	}
}

int main(int argc, char *argv[])
{
	// Create new config and initialize it
	cfg = new_cfg_init();

	// Test correctness of command line arguments
	if (test_args(argc, argv, cfg)) {
		destroy_cfg(cfg);
		call_error(ERR_ARGS);
	}

	// Catch the signal
    signal(SIGINT, sig_handler);

	// Create socket
	if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		destroy_cfg(cfg);
		call_error(ERR_SOCK);
	}

	// Initialize of server address
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(cfg->port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding
	if (bind(server_socket_fd, (struct sockaddr *) &server_address,
		sizeof(server_address)) < 0) {
		close(server_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_BIND);
	}

	// Listen
	if (listen(server_socket_fd, WAIT_SIZE) < 0) {
		close(server_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_LIST);
	}

	// Opening /etc/passwd
	if ((fr = fopen("/etc/passwd", "r")) == NULL) {
		close(server_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_FOPEN);
	}

	// Init things for accept message from client
 	struct sockaddr_in new_address;
	socklen_t new_address_size;
	memset(&new_address, '\0', sizeof(new_address));
	memset(&new_address_size, '\0', sizeof(new_address_size));

	char msg_recv[SIZE_OF_BUFFER] = {0};
	char msg_send[SIZE_OF_BUFFER * 50] = {0}; // MAXIMUM SIZE
	char buffer[SIZE_OF_BUFFER] = {0};

	// Communicate with client
	while (true) {
		fseek(fr, 0, SEEK_SET);
		memset(msg_recv, '\0', sizeof(msg_recv));
		memset(msg_send, '\0', sizeof(msg_send));

		// Accept connection
		if ((new_socket = accept(server_socket_fd, (struct sockaddr *)
		 	&new_address, &new_address_size)) < 0) {
			close(new_socket);
		 	close(server_socket_fd);
			destroy_cfg(cfg);
		 	call_error(ERR_ACCE);
		}

		// Fork, child take care of connected client, parent take care of potential other clients
		if (fork() == 0) {

			// Receive message
			if (recv(new_socket, msg_recv, SIZE_OF_BUFFER, 0) == -1) {
				close(server_socket_fd);
				close(new_socket);
				destroy_cfg(cfg);
				call_error(ERR_RECV);
			}

			// Decide what information client wants to get
			switch (msg_recv[0]) {
				case 'N': // name
					read_name(fr, msg_recv + sizeof(char), msg_send);
					if (send(new_socket, msg_send, SIZE_OF_BUFFER, 0) == -1) {
						close(server_socket_fd);
						close(new_socket);
						destroy_cfg(cfg);
						call_error(ERR_SEND);
					}
					break;

				case 'F': // home dir
					read_folder(fr, msg_recv + sizeof(char), msg_send);
					if (send(new_socket, msg_send, SIZE_OF_BUFFER, 0) == -1) {
						close(server_socket_fd);
						close(new_socket);
						destroy_cfg(cfg);
						call_error(ERR_SEND);
					}
					break;

				case 'L': // logins
					read_login(fr, msg_recv, msg_send);
					for (int i = 1, j = 0; i < (int) strlen(msg_send); i++) {
						buffer[j++] = msg_send[i - 1];
						if (i % (SIZE_OF_BUFFER-2) == 0) {
							if (send(new_socket, buffer, SIZE_OF_BUFFER, 0) == -1) {
								close(server_socket_fd);
								close(new_socket);
								destroy_cfg(cfg);
								call_error(ERR_SEND);
							}
							memset(buffer, '\0', sizeof(buffer));
							j = 0;
						}
					}
					strcat(buffer, " ");
					if (send(new_socket, buffer, SIZE_OF_BUFFER, 0) == -1) {
						close(server_socket_fd);
						close(new_socket);
						destroy_cfg(cfg);
						call_error(ERR_SEND);
					}
					break;
			}
			break; // Child jump out of the infinite loop
		}
	}

	return 0;
}
