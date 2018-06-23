// VUT FIT
// IPK 2017/2018
// Project 1
// Variant 1: Klient-server pro ziskani informace o uzivatelich
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 12/3/2018
// File: client.c

#include "client.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

Config *new_cfg_init()
{
	Config *cfg = calloc(1, sizeof(Config));
	if (cfg == NULL)
		call_error(ERR_ALLOC);
	cfg->host = NULL;
	cfg->login = NULL;
	return cfg;
}

void destroy_cfg(Config *cfg)
{
	if (cfg != NULL) {
		if (cfg->host != NULL) {
			free(cfg->host);
			cfg->host = NULL;
		}
		if (cfg->login != NULL) {
			free(cfg->login);
			cfg->login = NULL;
		}
		free(cfg);
		cfg = NULL;
	}
}

int test_args(int argc, char *argv[], Config *cfg)
{
	int opt;
	bool h = false, p = false, n = false, f = false, l = false;
	if (argc != 6 && argc != 7)
		return 1;

	while ((opt = getopt(argc, argv, "h:p:n:f:l")) != -1) {
		switch (opt) {
			case 'h':
				if (h)
					return 3;
				h = true;
				cfg->host = calloc(strlen(optarg) + 1, sizeof(char));
				if (cfg->host == NULL)
					call_error(ERR_ALLOC);
				strcpy(cfg->host, optarg);
				break;

			case 'p':
				if (p)
					return 3;
				p = true;
				for (int i = 0; i < (int) strlen(optarg); i++) {
					if (!isdigit(optarg[i]))
					return 3;
				}
				cfg->port = atoi(optarg);
				break;

			case 'n':
				if (n || f || l)
					return 3;
				n = true;
				cfg->arg3 = N;
				cfg->login = calloc(strlen(optarg) + 1, sizeof(char));
				if (cfg->login == NULL)
					call_error(ERR_ALLOC);
				strcpy(cfg->login, optarg);
				break;

			case 'f':
				if (n || f || l)
					return 3;
				f = true;
				cfg->arg3 = F;
				cfg->login = calloc(strlen(optarg) + 1, sizeof(char));
				if (cfg->login == NULL)
					call_error(ERR_ALLOC);
				strcpy(cfg->login, optarg);
				break;

			case 'l':
				if (n || f || l)
					return 3;
				l = true;
				cfg->arg3 = L;
				if (argc == 7) {
					cfg->login = calloc(strlen(argv[optind]) + 1, sizeof(char));
					if (cfg->login == NULL)
					call_error(ERR_ALLOC);
					strcpy(cfg->login, argv[optind]);
				}
				else
					cfg->login = NULL;
				break;

			default:
				return 2;
		}
	}
	if (cfg->login != NULL) {
		if (strlen(cfg->login) > SIZE - 2) { // '\0' a znak pro ARG3
			return 4;
		}
	}
	return 0;
}

int host_to_ip(char *host, char *ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(host)) == NULL) {
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    memset(ip, '\0', sizeof(*ip));
    for (int i = 0; addr_list[i] != NULL; i++) {
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }
    return 2;
}

void convert_msg(Config *cfg, char *msg)
{
	if (cfg->login != NULL) {
		strcpy(msg + sizeof(char), cfg->login);
		switch (cfg->arg3) {
			case N:
				msg[0] = 'N';
				break;
			case F:
				msg[0] = 'F';
				break;
			case L:
				msg[0] = 'L';
				break;
		}
	}
	else {
		msg[0] = 'L';
	}
}

int main(int argc, char *argv[])
{
	// Create new config and initialize it
	Config *cfg = new_cfg_init();

	// Test correctness of command line arguments
	if (test_args(argc, argv, cfg) != 0) {
		destroy_cfg(cfg);
		call_error(ERR_ARGS);
	}

	// Get IP from domain name
    if (host_to_ip(cfg->host, cfg->host) != 0) {
    	destroy_cfg(cfg);
        call_error(ERR_HOST);
    }

	// Create socket
	int client_socket_fd;
	if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		destroy_cfg(cfg);
		call_error(ERR_SOCK);
	}

	// Initialize of server address
	struct sockaddr_in server_address;
	memset(&server_address, '\0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(cfg->port);
	if (inet_aton(cfg->host, &server_address.sin_addr) == 0) {
		close(client_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_HOST);
	}

	// Connect to the server
	if (connect(client_socket_fd, (const struct sockaddr *) &server_address,
		sizeof(server_address)) != 0) {
		close(client_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_CONN);
	}

	// Send message to the server
	char msg[SIZE] = {0};
	convert_msg(cfg, msg);
	if (send(client_socket_fd, msg, strlen(msg), 0) == -1) {
		close(client_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_SEND);
	}

	// Receive message from the server
	bool rec_msg = false;
	do {
		memset(msg, '\0', sizeof(msg));
		if (recv(client_socket_fd, msg, SIZE, 0) == -1) {
			close(client_socket_fd);
			destroy_cfg(cfg);
			call_error(ERR_RECV);
		}
		if (strcmp(msg, " ") != 0) {
			printf("%s", msg);
			rec_msg = true;
		}
	} while (msg[strlen(msg)-1] != ' ');

	if (rec_msg == false) {
		close(client_socket_fd);
		destroy_cfg(cfg);
		call_error(ERR_NO_MATCH);
	}
	else
		putchar('\n');

	// Close socket
	close(client_socket_fd);

	// Free config
	destroy_cfg(cfg);

	return 0;
}
