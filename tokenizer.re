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

#include "log.h"
#include "tokenizer.h"
#include "parser.h"

/* RE Format parser defines */
#define	YYCTYPE		char
#define	YYCURSOR	cursor
#define	YYMARKER	marker

/*!re2c /*!ignore!re2c Set RE2C Options */
	re2c:yyfill:enable=0;
*/

/*!re2c /*!ignore!re2c Useful Regular Expressions */
	EOI = [\000];
	EOL = [\012] | [\015];
	ANY	= [\000-\377];
	ALPHA	= [a-zA-Z];
	DIGIT	= [0-9];
	NUMBER	= (DIGIT+ | "." DIGIT+ | DIGIT+ "." DIGIT+) ('e' DIGIT+)?;
	TEXT = (ALPHA | DIGIT);
	WSP = (" ") | [\011];
	CONTINUE = EOL+ "+";
*/

static token_ * newToken(int type, double *variable, double constant)
{
	token_ *r;
	r = malloc(sizeof(token_));
	ReturnNULLIf(r == NULL, "Malloc Failed");
	r->type = type;
	r->variable = variable;
	r->constant = constant;
	r->next = NULL;
	return r;
}

int tokenizerDestroy(token_ **r)
{
	token_ *ptr, *ptrNext;
	
	ReturnErrIf(r == NULL, "Bad Pointer");
	ReturnErrIf((*r) == NULL, "Not initialised");
	Debug("Destroying Tokenizer %p", (*r));
	
	for(ptr = (*r); ptr != NULL; ptr = ptrNext) {
		ptrNext = ptr->next;
		free(ptr);
	}
	
	*r = NULL;
	return 0;
}

#define Attach(tok, var, con) \
	tail->next = newToken(tok, var, con); \
	GotoFailedIf(tail->next == NULL); \
	tail = tail->next

token_ * tokenizerNew(char *buffer, tokenizerGetDataPtr dataFunc, void *private)
{
	token_ *head = NULL, *tail = NULL;
	char *cursor = NULL, *marker = NULL, *start = NULL, tmp0, tmp1, iv = 'v';
	double *variable;
	
	ReturnNULLIf(buffer == NULL);
	
	Debug("Creating tokenizer");
	cursor = buffer;
	marker = buffer;
	head = newToken(-1, NULL, 0);
	ReturnNULLIf(head == NULL);
	tail = head;
	
parse:
	start = cursor;
/*!re2c /*!ignore!re2c Parse Block */
	(WSP+ | CONTINUE)	{ goto parse; }
	EOI 	{ Attach(0, NULL, 0);						goto passed; }
	'diff(' { Attach(TOKEN_DIFF, NULL, 0);				goto parse; }
	',' 	{ Attach(TOKEN_COMMA, NULL, 0);				goto parse; }
	'v('	{ Attach(TOKEN_VOLTAGE, NULL, 0); iv='v';	goto parse_iv; }
	'i('	{ Attach(TOKEN_CURRENT, NULL, 0); iv='i';	goto parse_iv; }
	'abs('	{ Attach(TOKEN_ABS, NULL, 0);				goto parse; }
	'acosh('{ Attach(TOKEN_ACOSH, NULL, 0);			goto parse; }
	'acos('	{ Attach(TOKEN_ACOS, NULL, 0);				goto parse; }
	'asinh('{ Attach(TOKEN_ASINH, NULL, 0);			goto parse; }
	'asin('	{ Attach(TOKEN_ASIN, NULL, 0);				goto parse; }
	'atanh('{ Attach(TOKEN_ATANH, NULL, 0);			goto parse; }
	'atan('	{ Attach(TOKEN_ATAN, NULL, 0);				goto parse; }
	'cosh('	{ Attach(TOKEN_COSH, NULL, 0);				goto parse; }
	'cos('	{ Attach(TOKEN_COS, NULL, 0);				goto parse; }
	'exp('	{ Attach(TOKEN_EXP, NULL, 0);				goto parse; }
	'ln('	{ Attach(TOKEN_LN, NULL, 0);				goto parse; }
	'log('	{ Attach(TOKEN_LOG, NULL, 0);				goto parse; }
	'sinh('	{ Attach(TOKEN_SINH, NULL, 0);				goto parse; }
	'sin('	{ Attach(TOKEN_SIN, NULL, 0);				goto parse; }
	'sqrt('	{ Attach(TOKEN_SQRT, NULL, 0);				goto parse; }
	'tan('	{ Attach(TOKEN_TAN, NULL, 0);				goto parse; }
	'uramp('{ Attach(TOKEN_URAMP, NULL, 0);			goto parse; }
	'u('	{ Attach(TOKEN_U, NULL, 0);					goto parse; }
	"+" 	{ Attach(TOKEN_PLUS, NULL, 0);				goto parse; }
	"-" 	{ Attach(TOKEN_MINUS, NULL, 0);				goto parse; }
	"*" 	{ Attach(TOKEN_TIMES, NULL, 0);				goto parse; }
	"/" 	{ Attach(TOKEN_DIVIDE, NULL, 0);			goto parse; }
	"^" 	{ Attach(TOKEN_POWER, NULL, 0);				goto parse; }
	"(" 	{ Attach(TOKEN_LPAREN, NULL, 0);			goto parse; }
	")" 	{ Attach(TOKEN_RPAREN, NULL, 0);			goto parse; }
	NUMBER 	{ Attach(TOKEN_CONSTANT, NULL, atof(start)); goto parse; }
	ALPHA+ 	{ 
		tmp0 = *cursor;
		*cursor = 0x0;
		variable = dataFunc(start, private);
		Attach(TOKEN_VARIABLE, variable, 0);
		*cursor = tmp0;
		goto parse;
	}
	ANY 	{ Error("Unexpected character %s", cursor-1);	goto failed; }
*/

parse_iv:
	start = cursor;
/*!re2c /*!ignore!re2c Parse Block */
	(WSP+ | CONTINUE)	{ goto parse_iv; }
	")" 	{ Attach(TOKEN_RPAREN, NULL, 0);		goto parse; }
	','		{  Attach(TOKEN_COMMA, NULL, 0);		goto parse_iv; }
	(ALPHA | DIGIT)+ 	{ 
		tmp0 = *cursor;
		tmp1 = *(start - 1);
		*cursor = 0x0;
		*(start - 1) = iv;
		variable = dataFunc(start-1, private);
		Attach(TOKEN_VARIABLE, variable, 0);
		*cursor = tmp0;
		*(start - 1) = tmp1;
		goto parse_iv;
	}
	ANY 	{ cursor--; Error("Unexpected character %s", cursor);	goto failed; }
*/

passed:
	tail = head->next;
	free(head);
	return tail;
failed:
	tokenizerDestroy(&head);
	return NULL;
}
