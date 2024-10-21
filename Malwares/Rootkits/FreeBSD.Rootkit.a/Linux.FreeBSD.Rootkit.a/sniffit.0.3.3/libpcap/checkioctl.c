/*
 * Copyright (c) 1994, 1995
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* @(#) $Header: checkioctl.c,v 1.3+ 95/04/08 03:14:53 leres Exp $ (LBL) */

/*
 * checkioctl - check to see if fixincludes has been run
 *
 * The problem we're trying to detect is that we're using gcc but
 * fixincludes hasn't been run. Pre-ansi ioctl macros usually
 * look something like:
 *
 *     #define _IO(x, y)	(IOC_VOID | ('x' << 8) | y)
 *
 * while ansi ioctl macros look like:
 *
 *     #define _IO(x, y)	(IOC_VOID | (x << 8) | y)
 *
 * The difference being that in ansi, you can't quote macro arguments.
 * Notice that if you use the pre-ansi macro with ansi code, the first
 * argument is never expanded; it always comes out as 'x' or whatever.
 * This program detects this.
 */

#include <sys/types.h>
#include <sys/time.h>
#ifndef SOLARIS
#include <sys/ioctl.h>
#else
#include <sys/ioccom.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcap-int.h"

#ifdef __GNUC__
int main(int, char **);
#endif

int
main(argc, argv)
	int argc;
	char **argv;
{
	register char *prog;

	if ((prog = strrchr(argv[0], '/')) != NULL)
		++prog;
	else
		prog = argv[0];

#ifdef __GNUC__
	if (_IO('A', 1) == _IO('B', 1)) {
		fprintf(stderr,
		    "%s: You are using gcc and need to run fixincludes!\n",
		    prog);
		exit(1);
	}
#else
	printf("%s: You are not using gcc\n", prog);
#endif
	exit(0);
}
