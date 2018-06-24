// VUT FIT
// IPK 2017/2018
// Project 2
// Variant 3: DNS Lookup nastroj
// Author: Vladimir Dusek, xdusek27 (2BIT)
// Date: 9/4/2018
// File: main.cpp

#include "lookup.h"
#include "error.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// Parse command line arguments
	Config cfg;
	cfg.parse_options(argc, argv);

	// Convert arguments to C notation
	int record_type = (int) cfg.type;
	char *host_name = (char *) calloc(strlen(cfg.name.c_str()) + 1, sizeof(char));
	char *dns_server = (char *) calloc(strlen(cfg.server.c_str()) + 1, sizeof(char));
	if (host_name == NULL || dns_server == NULL)
		error(ERR_SYS_ALLOC);
	int timeout_sec = cfg.timeout;
	bool iterative = cfg.iterative_ex;
	strcpy(host_name, cfg.name.c_str());
	strcpy(dns_server, cfg.server.c_str());

	// Call iterative or recursive way of querying
	int ret;
	if (iterative)
		ret = iterative_way_querying(dns_server, timeout_sec, record_type, host_name);
	else
		ret = recursive_way_querying(dns_server, timeout_sec, record_type, host_name);

	// If satisfying answer
	if (ret == 0)
		return 0;
	else
		return 1;
}
