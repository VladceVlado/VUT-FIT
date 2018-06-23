// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: config.h

#pragma once

#include <iostream>

/**
 *  Types of DNS query. Values are set according to
 *  https://en.wikipedia.org/wiki/List_of_DNS_record_types .
 */
enum Type {
	T_A = 1,     // Address record
	T_AAAA = 28, // IPv6 address record
	T_NS = 2,    // Name server record
	T_PTR = 12,  // Pointer record
	T_CNAME = 5  // Canonical name record
};

/**
 * Parsing command line options. Print help on stdout if it is required or
 * call error() if there is a problem.
 */
class Config {
private:
	bool help_ex;
	bool server_ex;
	bool timeout_ex;
	bool type_ex;
	bool name_ex;
public:
	bool iterative_ex;  // iterative / recursive way of querying
	std::string server; // DNS server where we send queries
	int timeout;        // timeout
	Type type;          // type of DNS query
	std::string name;   // the name we ask for

	/**
	 * Constructor, set all attributes to default values.
	 */
	Config();

	/**
	 * Destructor.
	 */
	~Config();

	/**
	 * Parse command line arguments. Call print_help() if it is required or
	 * call error() if there is a problem.
	 */
	void parse_options(int argc, char *argv[]);

	/**
	 * Print help on stdout.
	 */
	void print_help();

	/**
	 * Print all attributes on stderr for purpose of debugging.
	 */
	void print();
};
