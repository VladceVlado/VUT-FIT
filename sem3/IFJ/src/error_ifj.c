#include "error_ifj.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR_MSG(ERROR) fprintf(stderr, "%s %s\n", "ifj17:", ERROR)
#define ERROR_MSG_EXT(ERROR, MSG) fprintf(stderr, "%s %s %s\n", "ifj17:", ERROR, MSG)

void errorExit(tERROR error, char *msg)
{
	bool extended = false;

	if (msg != NULL)
		extended = true;

	switch (error) {
		case E_OK:
			exit(0);

		case E_LEX:
			extended ? ERROR_MSG_EXT("lexical error:", msg) :
			           ERROR_MSG("lexical error");
			exit(1);

		case E_SYN:
			extended ? ERROR_MSG_EXT("syntax error:", msg) :
			           ERROR_MSG("syntax error");
			exit(2);

		case E_SEM_DEF:
			extended ? ERROR_MSG_EXT("sematic error:", msg) :
			           ERROR_MSG("sematic error: attempt to redefine/undefined variable/function");
			exit(3);

		case E_SEM_COMPA:
			extended ? ERROR_MSG_EXT("sematic error:", msg) :
			           ERROR_MSG("sematic error: incompatible types");
			exit(4);

		case E_SEM:
			extended ? ERROR_MSG_EXT("sematic error:", msg) :
			           ERROR_MSG("sematic error");
			exit(6);

		case E_SYS:
			extended ? ERROR_MSG_EXT("system error:", msg) :
			           ERROR_MSG("system error");
			exit(99);
	}
}
