/* 
 * Copyright (C) 2005 Cooper Street Innovations Inc.
 *	Charles Eidsness    <charles@cooper-street.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
 * 02110-1301, USA.
 *
 */

#ifndef LOG_H
#define LOG_H

#ifndef MESSAGE_LEVEL
#	define MESSAGE_LEVEL	4
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define M2STRX(a)	#a
#define M2STR(a)	M2STRX(a)
#define __LINESTR__	M2STR(__LINE__)

#if MESSAGE_LEVEL > 0
extern FILE *fderr;
#	define LogMaster \
	FILE *fderr = NULL; \
	FILE *fdlog = NULL
#	define OpenErrorFile(file) \
	fderr = fopen(file, "w")
#	define CloseErrorFile \
	if(fderr != NULL) { \
		fclose(fderr); \
		fderr = NULL; \
	}
#	define Error(args...) \
		fprintf(fderr ? fderr : stderr, \
				"ERROR: "__FILE__":"__LINESTR__"\t" args);\
		fprintf(fderr ? fderr : stderr, "\n");\
		(void)fflush(fderr ? fderr : stderr)
#else
#	define LogMaster
#	define OpenErrorFile(file)
#	define CloseErrorFile
#	define Error(args...)
#endif

#if MESSAGE_LEVEL > 1
#	define Warn(args...) \
		fprintf(fderr ? fderr : stderr, \
				"WARNING: "__FILE__":"__LINESTR__"\t" args);\
		fprintf(fderr ? fderr : stderr, "\n");\
		(void)fflush(fderr ? fderr : stderr)
#else
#	define Warn(args...)
#endif

#if MESSAGE_LEVEL > 2
extern FILE *fdlog;
#	define OpenLogFile(file) \
	fdlog = fopen(file, "w")
#	define CloseLogFile \
	if(fdlog != NULL) { \
		fclose(fdlog); \
		fdlog = NULL; \
	}
#	define Info(args...) \
		fprintf(fdlog ? fdlog : stdout, args);\
		fprintf(fdlog ? fdlog : stdout, "\n");\
		(void)fflush(fdlog ? fdlog : stdout)
#else
#	define OpenLogFile(file)
#	define CloseLogFile
#	define Info(args...)
#endif

#if MESSAGE_LEVEL > 3
#	define Debug(args...) \
		fprintf(fdlog ? fdlog : stdout, \
				"DEBUG: "__FILE__":"__LINESTR__"\t" args);\
		fprintf(fdlog ? fdlog : stdout, "\n");\
		(void)fflush(fdlog ? fdlog : stdout)
#	define Mark \
		fprintf(fdlog ? fdlog : stdout, \
				">>>> MARK: "__FILE__":"__LINESTR__ "<<<<");\
		fprintf(fdlog ? fdlog : stdout, "\n");\
		(void)fflush(fdlog ? fdlog : stdout)
#	define Text(args...) \
		fprintf(fdlog ? fdlog : stdout, args);	
#else
#	define Debug(args...)
#	define Text(args...)
#	define Mark
#endif

#define ReturnErrIf(expr, args...) \
	if(expr) {\
		Error(#expr " -- " args);\
		return -1;\
	}

#define ReturnErr(args...) \
	Error(args);\
	return -1

#define ReturnNULLIf(expr, args...) \
	if(expr) {\
		Error(#expr " -- " args);\
		return NULL;\
	}

#define ReturnNULL(args...) \
	Error(args);\
	return NULL

#define ReturnNULLAndFreeIf(ptr, expr, args...) \
	if(expr) {\
		Error(#expr " -- " args);\
		free(ptr);\
		return NULL;\
	}

#define GotoFailedIf(expr, args...) \
	if (expr) {\
		Error(#expr " -- " args);\
		goto failed;\
	}

#define ExitFailureIf(expr, args...) \
	if(expr) {\
		Error(#expr " -- " args);\
		exit(EXIT_FAILURE);\
	}

#define ExitFailure(args...) \
	Error(args);\
	exit(EXIT_FAILURE)

#define ExitSuccess \
	exit(EXIT_SUCCESS)

#endif
