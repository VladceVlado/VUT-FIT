// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: lookup.h

#pragma once

/**
 * Macro for debug purpose, print debug message on stderr
 * only if macro DEBUG is defined.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) do { std::cerr << x; } while (0)
#else
#define DEBUG_PRINT(x) do { } while (0)
#endif

/**
 * Recursive way of querying.
 */
int recursive_way_querying(char *dns_server, int timeout_sec, int record_type, char *host_name);

/**
 * Iterative way of querying. Not implemented (yet).
 */
int iterative_way_querying(char *dns_server, int timeout_sec, int record_type, char *host_name);

/**
 * Return random number.
 */
int get_random_number();

/**
 * Set the header of dns message. Return length of the header in bytes.
 */
int set_header(char *hstart, bool iterative);

/**
 * Set the question of dns message. Return length of the question in bytes.
 */
int set_question(char *qstart, char *host_name, int record_type);

/**
 * Convert url to dns format. For example www.fit.vutbr.cz will convert to
 * 3www3fit5vutbr2cz0. Numbers are values of bytes not characters.
 * Inspired by: https://gist.github.com/fffaraz/9d9170b57791c28ccda9255b48315168
 */
void url_to_dns(char *url, char *dns);

/**
 * Convert ip to ptr record form.
 * According to https://en.wikipedia.org/wiki/Reverse_DNS_lookup .
 */
void ip_to_ptr_record(char *ip, char *ptr_record);

/**
 * Read name in dns format and convert it back.
 * Inspired by: https://gist.github.com/fffaraz/9d9170b57791c28ccda9255b48315168
 */
int read_name(char *reader, char *buffer, char *name);

/**
 * Print required output on stdout.
 */
void print_output(char *name, int type, char *ans);

#pragma pack(push, 1) // exact fit - no padding

/**
 * Fields of the DNS header structure.
 */
struct Header {
    unsigned short id;         // id number for match up replies with queries
    char rd :1;                // recursion desired
    char tc :1;                // truncated message
    char aa :1;                // authoritive answer
    char opcode :4;            // purpose of message, 0 for standard query
    char qr :1;                // query (0) / response (1) flag
	char rcode :4;             // response code, 0 if no error occurred
    char z :3;                 // not used, reserved for the future
    char ra :1;                // recursion available
    unsigned short q_count;    // number of questions in the questions section of the message
    unsigned short ans_count;  // number of resource records in the answers section of the message
    unsigned short auth_count; // number of resource records in the authority section of the message
    unsigned short add_count;  // number of resource records in the additional section of the message
};

/**
 * Fields of query structure.
 */
struct Question {
	// + name of the node whose resource records are being requested
    unsigned short qtype;  // type of the requested resource records
    unsigned short qclass; // class of the requested resource records
};

/**
 * Fields of the resource record structure.
 */
struct Resource_record {
	// + encoded name of the node to which this resource record applies
    unsigned short rrtype;   // type of this resource record
    unsigned short rrclass;  // class of this resource record
    unsigned int ttl;        // number of seconds after which this record is no longer valid
    unsigned short data_len; // length in bytes of the data that follows
	// + specific record data
};

#pragma pack(pop) // back to the previous packing mode
