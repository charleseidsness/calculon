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

#include <math.h>

#include "log.h"
#include "calc.h"
#include "tokenizer.h"
#include "parser.h"

struct _calc {
	token_ *tokens;
	void* parser;
};

int calcSolve(calc_ *r, double *solution)
{
	token_ *ptr;
	double *diffVariable = NULL;
	parserArg arg;
	
	ReturnErrIf(r == NULL);
	ReturnErrIf(solution == NULL);
	
	arg.diffVariable = &diffVariable;
	arg.solution = solution;
	
	for(ptr = r->tokens; ptr != NULL; ptr = ptr->next) {
		Debug("%s", ParseTokenName(ptr->type));
		Parse(r->parser, ptr->type, ptr, arg);
		ReturnErrIf(isnan(*solution), "Parser failed");
	}
	
	return 0;
}

int calcDiff(calc_ *r, double *variable, double *solution)
{
	token_ *ptr;
	parserArg arg;
	
	ReturnErrIf(r == NULL);
	ReturnErrIf(solution == NULL);
	
	arg.diffVariable = &variable;
	arg.solution = solution;
	
	Debug("%s", ParseTokenName(TOKEN_DIFF));
	Parse(r->parser, TOKEN_DIFF, NULL, arg);
	ReturnErrIf(isnan(*solution), "Parser failed");
	
	Debug("%s", ParseTokenName(TOKEN_COMMA));
	Parse(r->parser, TOKEN_COMMA, NULL, arg);
	ReturnErrIf(isnan(*solution), "Parser failed");
	
	for(ptr = r->tokens; ptr != NULL; ptr = ptr->next) {
		if(ptr->type == 0) {
			Debug("%s", ParseTokenName(TOKEN_RPAREN));
			Parse(r->parser, TOKEN_RPAREN, NULL, arg);
			ReturnErrIf(isnan(*solution), "Parser failed");
		}
		Debug("%s", ParseTokenName(ptr->type));
		Parse(r->parser, ptr->type, ptr, arg);
		ReturnErrIf(isnan(*solution), "Parser failed");
	}
	
	return 0;
}

/* TODO: This could be made way more efficient by utalizing a
	special zero crossing parser */
int calcZeroXing(calc_ *r, double *solution)
{
	token_ *ptr;
	double *diffVariable = NULL, zero = 0.0;
	parserArg arg;
	token_ variable = {
		.type = TOKEN_VARIABLE,
		.variable = &zero,
		.constant = 0.0,
		.next = NULL,
	};
	
	ReturnErrIf(r == NULL);
	ReturnErrIf(solution == NULL);
	
	arg.diffVariable = &diffVariable;
	arg.solution = solution;
	
	for(ptr = r->tokens; ptr != NULL; ptr = ptr->next) {
		Debug("%s", ParseTokenName(ptr->type));
		if(ptr->type == TOKEN_VARIABLE) {
			Parse(r->parser, TOKEN_VARIABLE, &variable, arg);
		} else {
			Parse(r->parser, ptr->type, ptr, arg);
		}
		ReturnErrIf(isnan(*solution), "Parser failed");
	}
	
	return 0;
}

static int init(calc_ *r, char *buffer, calGetDataPtr dataFunc, void *private)
{
	Debug("Creating calculon %p", r);
	
	r->tokens = tokenizerNew(buffer, dataFunc, private);
	ReturnErrIf(r->tokens == NULL, "Failed to parse string");
	
	r->parser = ParseAlloc(malloc);
	ReturnErrIf(r->parser == NULL, "Failed to create parser");
	
	return 0;
}

int calcInfo(void)
{
	Info("calculon %i.%i", CALC_MAJOR_VERSION, CALC_MINOR_VERSION);
	Info("Compiled " __DATE__ " at " __TIME__);
	Info("(c) 2006 Cooper Street Innovations Inc.");
	return ((CALC_MAJOR_VERSION << 16) + CALC_MINOR_VERSION);
}

int calcDestroy(calc_ **r)
{
	ReturnErrIf(r == NULL);
	ReturnErrIf(*r == NULL);
	Debug("Destroying calculon %p", *r);
	
	if((*r)->tokens != NULL)
		tokenizerDestroy(&(*r)->tokens);
	if((*r)->parser != NULL)
		ParseFree((*r)->parser, free);
	
	free(*r);
	*r = NULL;
	return 0;
}

calc_ * calcNew(char *buffer, calGetDataPtr dataFunc, void *private)
{
	calc_ *r;
	ReturnNULLIf(buffer == NULL);
	ReturnNULLIf(dataFunc == NULL);
	r = malloc(sizeof(calc_));
	ReturnNULLIf(r == NULL, "Malloc Failed");
	memset(r, 0x0, sizeof(calc_));
	if(init(r, buffer, dataFunc, private)) {
		calcDestroy(&r);
	}
	return r;
}
