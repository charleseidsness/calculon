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
        
#ifndef TOKENIZER_H
#define TOKENIZER_H

/* should be in parser.h but put it here because 
	parser.h is auto-generated */
typedef struct {
	double **diffVariable;
	double *solution;
} parserArg;

typedef struct _token token_;
struct _token {
	int type;
	double *variable;
	double constant;
	token_ *next;
};

typedef double * (*tokenizerGetDataPtr)(char *varName, void *private);

int tokenizerDestroy(token_ **r);
token_ * tokenizerNew(char *buffer, tokenizerGetDataPtr dataFunc, void *private);

#endif
