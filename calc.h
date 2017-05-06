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
        
#ifndef CALC_H
#define CALC_H

#define CALC_MAJOR_VERSION		1
#define CALC_MINOR_VERSION		0

typedef struct _calc calc_;

typedef double * (*calGetDataPtr)(char *varName, void *private);

int calcSolve(calc_ *r, double *solution);
int calcDiff(calc_ *r, double *variable, double *solution);
int calcZeroXing(calc_ *r, double *solution);

int calcInfo(void);
int calcDestroy(calc_ **r);
calc_ * calcNew(char *buffer, calGetDataPtr dataFunc, void *private);

#endif
