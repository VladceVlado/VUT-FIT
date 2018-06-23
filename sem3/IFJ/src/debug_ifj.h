/**
 * @file debug_ifj17.h
 * @brief If the project is build for the debug, the debug macros will be defined.
 */

#ifndef _IFJ17_DEBUG_H
#define _IFJ17_DEBUG_H

#ifndef IFJ17_DEBUG
	#define DEBUG_PRINT(FORMAT, ...) ((void)0)
	#define DEBUG_MESSAGE(MSG) ((void)0)
#else
	#define DEBUG_PRINT(FORMAT, ...) \
				fprintf(stderr, "%s %s() in %s, line %i: " FORMAT "\n", \
				"DEBUG:", __func__, __FILE__, __LINE__, __VA_ARGS__)
	#define DEBUG_MESSAGE(MSG) DEBUG_PRINT("%s", MSG)
#endif

#endif /*_IFJ17_DEBUG_H*/
