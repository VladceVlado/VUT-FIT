// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: config.cpp

#include "config.h"
#include "error.h"
#include <string.h>
#include <getopt.h>
#include <iostream>
#include <stdexcept>

using namespace std;

Config::Config():
	help_ex(false),
	server_ex(false),
	timeout_ex(false),
	type_ex(false),
	name_ex(false),
	iterative_ex(false),
	server(""),
	timeout(5),
	type(T_A),
	name("")
{
}

Config::~Config()
{
}

void Config::print_help()
{
	cout << "Usage:" << endl;
	cout << "./ipk-lookup [-h]" << endl;
	cout << "./ipk-lookup -s server [-T timeout] [-t type] [-i] name" << endl;
	cout << "    <server> is IPv4 address" << endl;
	cout << "    <timeout> is integer (5 default)" << endl;
	cout << "    <type> e {A (default), AAAA, NS, PTR, CNAME}" << endl;
	cout << "    -i for iterative way of querying (recursive default)" << endl;
	cout << "    <name> is a domain name we ask for" << endl;
}

void Config::print()
{
	cerr << "##### CONFIG #####" << endl;
	cerr << "help_ex = " << help_ex << endl;
	cerr << "server_ex = " << server_ex << endl;
	cerr << "timeout_ex = " << timeout_ex << endl;
	cerr << "type_ex = " << type_ex << endl;
	cerr << "iterative_ex = " << iterative_ex << endl;
	cerr << "name_ex = " << name_ex << endl;
	cerr << "server = " << server << endl;
	cerr << "name = " << name << endl;
	cerr << "timeout = " << timeout << endl;
	cerr << "type = " << type << endl;
	cerr << "##################" << endl;
}

void Config::parse_options(int argc, char *argv[])
{
	if (argc < 2 || argc > 9)
		error(ERR_INV_OPTS);

	int opt;
	while ((opt = getopt (argc, argv, "hs:T:t:i")) != EOF) {
		switch (opt) {
			case 'h':
				help_ex = true;
				break;

			case 's':
				server_ex = true;
				server = optarg;
				break;

			case 'T':
				timeout_ex = true;
				try {
					timeout = stoi(optarg);
				}
				catch (std::exception &e) {
					error(ERR_INV_TIMEOUT);
				}
				break;

			case 't':
				type_ex = true;
				if (strcmp(optarg, "A") == 0)
					 type = T_A;
				else if (strcmp(optarg, "AAAA") == 0)
					 type = T_AAAA;
				else if (strcmp(optarg, "NS") == 0)
					 type = T_NS;
				else if (strcmp(optarg, "PTR") == 0)
					 type = T_PTR;
				else if (strcmp(optarg, "CNAME") == 0)
					 type = T_CNAME;
				else
					error(ERR_INV_TYPE);
				break;

			case 'i':
				iterative_ex = true;
				break;

			case '?':
				error(ERR_INV_OPTS);

			default:
				error(ERR_UNKNOWN);
		}
	}

	if (argv[optind] != nullptr) {
		if (strcmp(argv[optind], "") == 0)
			error(ERR_INV_OPTS);
		name_ex = true;
		name = argv[optind];
	}

	if (help_ex) {
		if (server_ex || timeout_ex || type_ex || iterative_ex || name_ex)
			error(ERR_INV_OPTS);
		else {
			print_help();
			exit(0);
		}
	}

	else {
		if (!server_ex || !name_ex)
			error(ERR_INV_OPTS);
	}
}
