// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: lookup.cpp

#include "lookup.h"
#include "error.h"
#include "config.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

using namespace std;

#define PORT 53
#define SIZE_OF_BUFFER 1024
#define SIZE_OF_MESSAGE 65536

int get_random_number()
{
	srand(time(NULL));
	return rand();
}

int set_header(char *hstart, bool iterative)
{
	Header *header = (Header *) hstart;
	header->id = htons(get_random_number()); // random number
	header->rd = (iterative == true) ? 0 : 1; // if iterative is set up
    header->tc = 0;
    header->aa = 0;
    header->opcode = 0;
	header->qr = 0;
    header->rcode = 0;
    header->z = 0;
    header->ra = 0;
    header->q_count = htons(1); // 1 question
    header->ans_count = 0;
    header->auth_count = 0;
    header->add_count = 0;

    return sizeof(Header);
}

int set_question(char *qstart, char *host_name, int record_type)
{
	if (record_type == T_PTR) {
		char buffer[SIZE_OF_BUFFER] = {0};
		ip_to_ptr_record(host_name, buffer);
	    url_to_dns(buffer, qstart);
	}
	else
		url_to_dns(host_name, qstart);

	int name_len = strlen(qstart) + 1;
	Question *question = (Question *) (qstart + name_len);
	question->qtype = htons(record_type);
	question->qclass = htons(1);

	return name_len + sizeof(Question);
}

void url_to_dns(char *url, char *dns)
{
    int lock = 0;
    strcat(url, ".");

    for (int i = 0; i < (int) strlen(url); i++) {
        if (url[i] == '.') {
            *(dns++) = i - lock;
            for (; lock < i; lock++)
                *(dns++) = url[lock];
            lock++;
        }
    }
    *dns = '\0';
}

void ip_to_ptr_record(char *ip, char *ptr_record)
{
	char result[SIZE_OF_BUFFER] = {0};
	char ip_reversed[8] = {0};

	// ipv4
	if (inet_pton(AF_INET, ip, ip_reversed) > 0) {
	    sprintf(result, "%u.%u.%u.%u.in-addr.arpa", ip_reversed[3],
	    	ip_reversed[2], ip_reversed[1], ip_reversed[0]);
	}
	// ipv6
	else if (inet_pton(AF_INET6, ip, ip_reversed) > 0) {
	    char buffer[5] = {0};
		for (int i = 31; i >= 0; i--) {
			if (i % 2 == 0)
			    sprintf(buffer, "%x.", (ip_reversed[i / 2] & 0xf0) >> 4);
			else
			    sprintf(buffer, "%x.", (ip_reversed[i / 2] & 0x0f) );
			strcat(result, buffer);
	    }
			strcat(result, "ip6.arpa");
	}
	// error
	else
		error(ERR_INV_PTR);

	strcpy(ptr_record, result);
}

int read_name(char *reader, char *buffer, char *name)
{
    int offset, name_len = 1, i = 0;
    bool jumped = false;
    char *old_reader = reader;

    while (*reader != '\0') {
		if (*reader >= 192) {
		    offset = (*reader - 192) * 256 + *(reader + 1);
		    reader = buffer + offset;
		    jumped = true;
		}
		else {
			name[i++] = *reader;
			if (!jumped)
			    name_len++;
			reader++;
		}
    }

    name[i] = '\0';
    if (jumped)
    	name_len++;

    for (int c, i = 0; i < (int) strlen(name); i++) {
    	c = name[i];
    	for (int j = 0; j < c; j++) {
    		name[i] = name[i + 1];
    		i++;
    	}
    	name[i] = '.';
    }

    name[i] = '\0';
    reader = old_reader;

    return name_len;
}

void print_output(char *name, int type, char *answer)
{
	struct in_addr address;
	char answer_str[SIZE_OF_BUFFER] = {0};

	switch (type) {
		case T_A:
			memcpy(&address, answer, 4);
			inet_ntop(AF_INET, &address, answer_str, SIZE_OF_BUFFER);
			cout << name << " IN A " << answer_str << endl;
			break;

		case T_AAAA:
			memcpy(&address, answer, 16);
			inet_ntop(AF_INET6, &address, answer_str, SIZE_OF_BUFFER);
			cout << name << " IN AAAA " << answer_str << endl;
			break;

		case T_NS:
			cout << name << " IN NS " << answer << endl;
			break;

		case T_PTR:
			cout << name << " IN PTR " << answer << endl;
			break;

		case T_CNAME:
			cout << name << " IN CNAME " << answer << endl;
			break;

		default:
			error(ERR_UNKNOWN);
	}
}

int recursive_way_querying(char *dns_server, int timeout_sec, int record_type, char *host_name)
{
	// Buffer for the message
	char buffer[SIZE_OF_MESSAGE] = {0};

	// Open socket, ipv4, udp
	int sock_fd;
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		error(ERR_SYS_SOCKET);

	// Initialization of dns_server address
	struct sockaddr_in dst;
	dst.sin_family = AF_INET;
	dst.sin_port = htons(PORT);
	if (inet_aton(dns_server, &dst.sin_addr) == 0)
		error(ERR_INV_SERVER);

	// Set up socket for timeout
	struct timeval timeout;
	timeout.tv_sec = timeout_sec;
	timeout.tv_usec = 0;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
	    error(ERR_SYS_SETSOCKET);
	if (setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
	    error(ERR_SYS_SETSOCKET);


	// Set header into message
	int header_size = set_header(buffer, false);

	// Set question into message
	int query_size = set_question(buffer + header_size, host_name, record_type);

	// Send message through the socket to the dns_server
	if (sendto(sock_fd, buffer, header_size + query_size, 0, (struct sockaddr *) &dst, sizeof(dst)) < 0)
	    error(ERR_SYS_SENDTO);

	// Receive the answer
	int size_dst = sizeof(dst);
	if (recvfrom(sock_fd, buffer, SIZE_OF_MESSAGE, 0, (struct sockaddr *) &dst, (socklen_t *) &size_dst) < 0)
	    error(ERR_TIMEOUT);

	// Read the answer
    Header *header = (Header*) buffer;
	Resource_record *resource_data = NULL;
	char name[SIZE_OF_BUFFER] = {0};
	char *reader = (char *) (buffer + header_size + query_size);
	bool type_ok = false;

	for (int i = 0; i < ntohs(header->ans_count); i++) {
		int name_len = read_name(reader, buffer, name);
		resource_data = (Resource_record *) (reader + name_len);

		// Print output
		switch (ntohs(resource_data->rrtype)) {
			case T_CNAME:
				read_name(reader + name_len + sizeof(Resource_record), buffer, name);
				print_output(host_name, T_CNAME, name);
				break;

			case T_PTR:
				cout << name;
				read_name(reader + name_len + sizeof(Resource_record), buffer, name);
				print_output((char *) "", T_PTR, name);
				break;

			case T_NS:
				cout << name;
				read_name(reader + name_len + sizeof(Resource_record), buffer, name);
				print_output((char *) "", T_NS, name);

			default:
				print_output(name, ntohs(resource_data->rrtype), reader + name_len + sizeof(Resource_record));
		}

		if (ntohs(resource_data->rrtype) == record_type)
            type_ok = true;

		reader += name_len + sizeof(Resource_record) + ntohs(resource_data->data_len);
	}

	// Close socket
	close(sock_fd);

	// If satisfying answer
	if (type_ok)
		return 0;
	else
		return 1;
}

int iterative_way_querying(char *dns_server, int timeout_sec, int record_type, char *host_name)
{
	(void) dns_server;
	(void) timeout_sec;
	(void) record_type;
	(void) host_name;

	cout << "Iterative way of querying is not implemented (yet)." << endl;

	// ToDo

	return 0;
}
