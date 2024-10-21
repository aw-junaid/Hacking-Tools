#  Copyright (c) 1993, 1994, 1995
# 	The Regents of the University of California.  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that: (1) source code distributions
#  retain the above copyright notice and this paragraph in its entirety, (2)
#  distributions including binary code include the above copyright notice and
#  this paragraph in its entirety in the documentation or other materials
#  provided with the distribution, and (3) all advertising materials mentioning
#  features or use of this software display the following acknowledgement:
#  ``This product includes software developed by the University of California,
#  Lawrence Berkeley Laboratory and its contributors.'' Neither the name of
#  the University nor the names of its contributors may be used to endorse
#  or promote products derived from this software without specific prior
#  written permission.
#  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
#  WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# @(#) $Header: Makefile.in,v 1.37+ 94/06/25 14:53:47 leres Exp $ (LBL)

#
# Various configurable paths
#
# where the installed pcap files go
INCL_DIR = /usr/local/include
# where libpcap.a should go
LIB_DIR = /usr/local/lib
# where man page should go
MAN_DIR = /usr/local/man
#
# Owner of installed files
#
BIN_GRP = bin
BIN_OWN = bin

#
# You shouldn't need to edit anything below.
#

#have-ethers#ETHERS_DEFINES = -DETHER_SERVICE

#have-sunos5#OS_DEFINES = -DSOLARIS

DEFINE_STDC = -D__STDC__
#have-gcc#DEFINE_STDC =
#have-irix4#OS_DEFINES = -Dvolatile= $(DEFINE_STDC)
#have-irix5#OS_DEFINES = -Dvolatile= $(DEFINE_STDC)

#have-pf#PCAP_DEFINES = -DPCAP_PF

#have-bpf#PCAP = bpf
#have-dlpi#PCAP = dlpi
#have-nit#PCAP = nit
#have-pf#PCAP = pf
#have-snit#PCAP = snit
#have-snoop#PCAP = snoop
PCAP = linux

CCOPT = -O
INCLUDES = -I.
INCLUDES = -I. -Ilinux-include
DEFINES = -DFDDI $(ETHERS_DEFINES) $(PCAP_DEFINES) $(OS_DEFINES)

# Standard CFLAGS
CFLAGS = $(CCOPT) $(DEFINES) $(INCLUDES)

# Standard C compiler
CC = cc
GCC_FLAGS = -Wmissing-prototypes -Wstrict-prototypes
#have-gcc1#GCC_FLAGS =
#have-gcc#CC = gcc -g -Wall $(GCC_FLAGS)

#
# Flex and bison allow you to specify the prefixes of the global symbols
# used by the generated parser.  This allows programs to use lex/yacc
# and link against libpcap.  If you don't have flex or bison, get them.
#
LEX = lex
YACC = yacc
#have-flex#LEX = flex -Ppcap_
#have-bison#YACC = bison -y -p pcap_

MAKE = make
SHELL = /bin/sh

RANLIB = true
RANLIB = ranlib

# Explicitly define compiliation rule since SunOS 4's make doesn't like gcc.
# Also, gcc does not remove the .o before forking 'as', which can be a
# problem if you don't own the file but can write to the directory.
.c.o:
	rm -f $@; $(CC) $(CFLAGS) -c $*.c

PCAPSRC = \
	pcap-$(PCAP).c
CSRC =	pcap.c inet.c gencode.c optimize.c nametoaddr.c etherent.c savefile.c \
	bpf_filter.c bpf_image.c
LSRC =	scanner.l
YSRC =	grammar.y
GENSRC = \
	$(LSRC:.l=.c) $(YSRC:.y=.c)

SRC =	$(PCAPSRC) $(CSRC) $(GENSRC)
ALLSRC = \
	pcap-bpf.c pcap-dlpi.c pcap-enet.c pcap-nit.c pcap-pf.c \
	pcap-snit.c pcap-snoop.c \
	$(CSRC) checkioctl.c $(LSRC) $(YSRC)

OBJ =	$(SRC:.c=.o)
# Some makes can't handle the above substitution
#have-broken-make#OBJ = pcap-$(PCAP).o \
#have-broken-make#	pcap.o inet.o gencode.o optimize.o nametoaddr.o \
#have-broken-make#	etherent.o savefile.o  bpf_filter.o bpf_image.o \
#have-broken-make#	scanner.o grammar.o

HDR =	pcap.h pcap-int.h pcap-namedb.h pcap-nit.h pcap-pf.h \
	ethertype.h gencode.h
GENHDR = \
	tokdefs.h

TAGFILES = \
	$(SRC) $(HDR)

TARFILES = \
	README CHANGES INSTALL VERSION Makefile.in pcap.3 mkdep \
	$(ALLSRC) $(HDR) \
	net bpf/net/bpf_filter.c bpf/net/bpf.h SUNOS4 configure

CLEANFILES = \
	$(OBJ) libpcap.a checkioctl lex.yy.c $(GENSRC) $(GENHDR)

libpcap.a: $(OBJ)
	rm -f libpcap.a
	ar rc $@ $(OBJ)
	$(RANLIB) $@

checkioctl: checkioctl.c
	rm -f $@; $(CC) $(CFLAGS) $@.c -o $@

scanner.c: scanner.l
	rm -f $@; $(LEX) -t $< > $$$$.$@; mv $$$$.$@ $@

scanner.o: scanner.c tokdefs.h
	rm -f $@; $(CC) $(CFLAGS) $(LEX_DEFINES) -c $*.c

tokdefs.h: grammar.c
grammar.c: grammar.y
	rm -f grammar.c tokdefs.h
	$(YACC) -d $<
	mv y.tab.c grammar.c
	mv y.tab.h tokdefs.h

grammar.o: grammar.c
	$(CC) -c $(CFLAGS) -Dyylval=pcap_lval grammar.c

install: force
	@for i in $(INCL_DIR) $(LIB_DIR) ; do \
		if [ ! -d $$i ] ; then \
			echo "mkdir $$i"; \
			mkdir $$i; \
			chmod 755 $$i; \
			chown $(BIN_OWN) $$i; \
			chgrp $(BIN_GRP) $$i; \
		else \
			true; \
		fi; \
	done
	cp libpcap.a $(LIB_DIR); \
	    chmod 444 $(LIB_DIR)/libpcap.a; \
	    chown $(BIN_OWN) $(LIB_DIR)/libpcap.a; \
	    chgrp $(BIN_GRP) $(LIB_DIR)/libpcap.a
	-$(RANLIB) $(LIB_DIR)/libpcap.a
	cp pcap.h pcap-namedb.h $(INCL_DIR); \
	    chmod 444 $(INCL_DIR)/pcap.h $(INCL_DIR)/pcap-namedb.h; \
	    chown $(BIN_OWN) $(INCL_DIR)/pcap.h $(INCL_DIR)/pcap-namedb.h; \
	    chgrp $(BIN_GRP) $(INCL_DIR)/pcap.h $(INCL_DIR)/pcap-namedb.h
	@if [ ! -f /usr/include/net/bpf.h ]; then \
		i=$(INCL_DIR)/net; \
		if [ ! -d $$i ] ; then \
			echo "mkdir $$i"; \
			mkdir $$i; \
			chmod 755 $$i; \
			chown $(BIN_OWN) $$i; \
			chgrp $(BIN_GRP) $$i; \
		else \
			true; \
		fi; \
		set -x; \
		cp net/bpf.h $(INCL_DIR)/net; \
		chmod 444 $(INCL_DIR)/net/bpf.h; \
		chown $(BIN_OWN) $(INCL_DIR)/net/bpf.h; \
		chgrp $(BIN_GRP) $(INCL_DIR)/net/bpf.h; \
	fi

install-man: force
	@i=$(MAN_DIR)/man3; \
	if [ ! -d $$i ] ; then \
		echo "mkdir $$i"; \
		mkdir $$i; \
		chmod 755 $$i; \
		chown $(BIN_OWN) $$i; \
		chgrp $(BIN_GRP) $$i; \
	else \
		true; \
	fi; \
	cp pcap.3 $(MAN_DIR)/man3
	chmod 444 $(MAN_DIR)/man3/pcap.3
	chown $(BIN_OWN) $(MAN_DIR)/man3/pcap.3
	chgrp $(BIN_GRP) $(MAN_DIR)/man3/pcap.3

clean:
	rm -f $(CLEANFILES)

realclean:
	rm -f $(CLEANFILES) Makefile tags

tags: $(TAGFILES)
	ctags -wtd $(TAGFILES)

tar:	force
	@cwd=`pwd` ; dir=`basename $$cwd` ; name=libpcap-`cat VERSION` ; \
	    list="" ; tar="tar cFFf" ; \
	    for i in $(TARFILES) ; do list="$$list $$name/$$i" ; done; \
	    echo \
	    "rm -f ../$$name; ln -s $$dir ../$$name" ; \
	     rm -f ../$$name; ln -s $$dir ../$$name ; \
	    echo \
	    "(cd .. ; $$tar - [lots of files]) | compress > /tmp/$$name.tar.Z" ; \
	     (cd .. ; $$tar - $$list) | compress > /tmp/$$name.tar.Z ; \
	    echo \
	    "rm -f ../$$name" ; \
	     rm -f ../$$name

force:	/tmp
depend:	$(GENSRC) force
	./mkdep -c $(CC) $(DEFINES) $(INCLUDES) $(SRC)
# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

pcap-linux.o: pcap-linux.c /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/limits.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/posix1_lim.h \
 /usr/include/linux/limits.h /usr/include/posix2_lim.h \
 /usr/include/linux/param.h /usr/include/asm/param.h \
 /usr/include/sys/types.h /usr/include/linux/types.h \
 /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h \
 /usr/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/netdb.h /usr/include/paths.h /usr/include/ctype.h \
 /usr/include/endian.h /usr/include/bytesex.h /usr/include/signal.h \
 /usr/include/linux/signal.h /usr/include/asm/signal.h \
 /usr/include/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/sys/time.h \
 /usr/include/linux/time.h /usr/include/time.h \
 /usr/include/sys/socket.h /usr/include/linux/socket.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/linux/sockios.h /usr/include/linux/uio.h \
 /usr/include/sys/file.h /usr/include/fcntl.h /usr/include/gnu/types.h \
 /usr/include/linux/fcntl.h /usr/include/asm/fcntl.h \
 /usr/include/sys/ioctl.h /usr/include/linux/ioctl.h \
 /usr/include/asm/ioctl.h /usr/include/termios.h \
 /usr/include/linux/termios.h /usr/include/asm/termios.h \
 /usr/include/asm/termbits.h /usr/include/asm/ioctls.h \
 /usr/include/sys/socketio.h /usr/include/malloc.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h \
 /usr/include/memory.h /usr/include/string.h /usr/include/unistd.h \
 /usr/include/posix_opt.h /usr/include/confname.h \
 /usr/include/net/if.h /usr/include/linux/if.h \
 /usr/include/net/if_arp.h /usr/include/linux/netdevice.h \
 /usr/include/linux/config.h /usr/include/linux/autoconf.h \
 /usr/include/linux/if_ether.h linux-include/linux/if_arp.h \
 /usr/include/netinet/in.h /usr/include/linux/in.h \
 /usr/include/asm/byteorder.h linux-include/netinet/in_systm.h \
 linux-include/netinet/ip.h linux-include/netinet/ip_var.h \
 linux-include/netinet/udp.h linux-include/netinet/tcp.h net/bpf.h \
 pcap-int.h pcap.h
pcap.o: pcap.c /usr/include/sys/types.h /usr/include/linux/types.h \
 /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h \
 /usr/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/unistd.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/posix_opt.h \
 /usr/include/gnu/types.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h \
 /usr/include/confname.h pcap-int.h pcap.h /usr/include/sys/time.h \
 /usr/include/linux/time.h /usr/include/time.h net/bpf.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
inet.o: inet.c /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/limits.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/posix1_lim.h \
 /usr/include/linux/limits.h /usr/include/posix2_lim.h \
 /usr/include/linux/param.h /usr/include/asm/param.h \
 /usr/include/sys/types.h /usr/include/linux/types.h \
 /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h \
 /usr/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/sys/file.h /usr/include/fcntl.h /usr/include/gnu/types.h \
 /usr/include/linux/fcntl.h /usr/include/asm/fcntl.h \
 /usr/include/sys/ioctl.h /usr/include/linux/ioctl.h \
 /usr/include/asm/ioctl.h /usr/include/termios.h \
 /usr/include/linux/termios.h /usr/include/asm/termios.h \
 /usr/include/asm/termbits.h /usr/include/asm/ioctls.h \
 /usr/include/sys/socketio.h /usr/include/linux/sockios.h \
 /usr/include/asm/sockios.h /usr/include/sys/socket.h \
 /usr/include/linux/socket.h /usr/include/asm/socket.h \
 /usr/include/linux/uio.h /usr/include/net/if.h \
 /usr/include/linux/if.h /usr/include/netinet/in.h \
 /usr/include/linux/in.h /usr/include/asm/byteorder.h \
 /usr/include/ctype.h /usr/include/endian.h /usr/include/bytesex.h \
 /usr/include/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/stdio.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/stdlib.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h \
 /usr/include/alloca.h /usr/include/string.h /usr/include/unistd.h \
 /usr/include/posix_opt.h /usr/include/confname.h pcap.h \
 /usr/include/sys/time.h /usr/include/linux/time.h /usr/include/time.h \
 net/bpf.h
gencode.o: gencode.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/socket.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/linux/socket.h /usr/include/asm/socket.h \
 /usr/include/asm/sockios.h /usr/include/linux/sockios.h \
 /usr/include/linux/uio.h /usr/include/sys/time.h \
 /usr/include/linux/time.h /usr/include/time.h /usr/include/net/if.h \
 /usr/include/linux/if.h net/bpf.h /usr/include/netinet/in.h \
 /usr/include/linux/in.h /usr/include/asm/byteorder.h \
 linux-include/netinet/if_ether.h /usr/include/net/if_arp.h \
 /usr/include/linux/netdevice.h /usr/include/linux/config.h \
 /usr/include/linux/autoconf.h /usr/include/linux/if_ether.h \
 linux-include/linux/if_arp.h /usr/include/memory.h \
 /usr/include/string.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h pcap.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 pcap-namedb.h /usr/include/setjmp.h /usr/include/jmp_buf.h \
 /usr/include/i386/jmp_buf.h /usr/include/signal.h \
 /usr/include/linux/signal.h /usr/include/asm/signal.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stdarg.h \
 /usr/include/stdlib.h /usr/include/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/alloca.h gencode.h
optimize.o: optimize.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/time.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/linux/time.h /usr/include/time.h net/bpf.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/memory.h /usr/include/string.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h gencode.h
nametoaddr.o: nametoaddr.c /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/limits.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/posix1_lim.h \
 /usr/include/linux/limits.h /usr/include/posix2_lim.h \
 /usr/include/linux/param.h /usr/include/asm/param.h \
 /usr/include/sys/types.h /usr/include/linux/types.h \
 /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h \
 /usr/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/sys/socket.h /usr/include/linux/socket.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/linux/sockios.h /usr/include/linux/uio.h \
 /usr/include/net/if.h /usr/include/linux/if.h \
 /usr/include/netinet/in.h /usr/include/linux/in.h \
 /usr/include/asm/byteorder.h linux-include/netinet/if_ether.h \
 /usr/include/net/if_arp.h /usr/include/linux/netdevice.h \
 /usr/include/linux/config.h /usr/include/linux/autoconf.h \
 /usr/include/linux/if_ether.h linux-include/linux/if_arp.h \
 /usr/include/arpa/inet.h /usr/include/ctype.h /usr/include/endian.h \
 /usr/include/bytesex.h /usr/include/errno.h \
 /usr/include/linux/errno.h /usr/include/asm/errno.h \
 /usr/include/netdb.h /usr/include/paths.h pcap.h \
 /usr/include/sys/time.h /usr/include/linux/time.h /usr/include/time.h \
 net/bpf.h /usr/include/stdio.h /usr/include/libio.h \
 /usr/include/_G_config.h pcap-namedb.h gencode.h ethertype.h
etherent.o: etherent.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/ctype.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/endian.h /usr/include/bytesex.h pcap.h \
 /usr/include/sys/time.h /usr/include/linux/time.h /usr/include/time.h \
 net/bpf.h /usr/include/stdio.h /usr/include/libio.h \
 /usr/include/_G_config.h pcap-namedb.h
savefile.o: savefile.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/time.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/linux/time.h /usr/include/time.h net/bpf.h \
 /usr/include/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/memory.h /usr/include/string.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/stdlib.h /usr/include/alloca.h /usr/include/unistd.h \
 /usr/include/posix_opt.h /usr/include/gnu/types.h \
 /usr/include/confname.h pcap-int.h pcap.h
bpf_filter.o: bpf_filter.c /usr/include/sys/param.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/limits.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/syslimits.h \
 /usr/include/limits.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/posix1_lim.h \
 /usr/include/linux/limits.h /usr/include/posix2_lim.h \
 /usr/include/linux/param.h /usr/include/asm/param.h \
 /usr/include/sys/types.h /usr/include/linux/types.h \
 /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h \
 /usr/include/asm/types.h /usr/include/sys/bitypes.h \
 /usr/include/sys/time.h /usr/include/linux/time.h /usr/include/time.h \
 net/bpf.h
bpf_image.o: bpf_image.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/time.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/linux/time.h /usr/include/time.h net/bpf.h pcap.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 /usr/include/string.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h
scanner.o: scanner.c /usr/include/stdio.h /usr/include/features.h \
 /usr/include/sys/cdefs.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/time.h \
 /usr/include/linux/time.h /usr/include/time.h /usr/include/ctype.h \
 /usr/include/endian.h /usr/include/bytesex.h pcap.h net/bpf.h \
 pcap-namedb.h gencode.h tokdefs.h /usr/include/stdlib.h \
 /usr/lib/gcc-lib/i486-linux/2.7.2/include/stddef.h \
 /usr/include/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/alloca.h
grammar.o: grammar.c /usr/include/sys/types.h \
 /usr/include/linux/types.h /usr/include/linux/posix_types.h \
 /usr/include/asm/posix_types.h /usr/include/asm/types.h \
 /usr/include/sys/bitypes.h /usr/include/sys/time.h \
 /usr/include/features.h /usr/include/sys/cdefs.h \
 /usr/include/linux/time.h /usr/include/time.h \
 /usr/include/sys/socket.h /usr/include/linux/socket.h \
 /usr/include/asm/socket.h /usr/include/asm/sockios.h \
 /usr/include/linux/sockios.h /usr/include/linux/uio.h \
 /usr/include/net/if.h /usr/include/linux/if.h net/bpf.h \
 /usr/include/netinet/in.h /usr/include/linux/in.h \
 /usr/include/asm/byteorder.h linux-include/netinet/if_ether.h \
 /usr/include/net/if_arp.h /usr/include/linux/netdevice.h \
 /usr/include/linux/config.h /usr/include/linux/autoconf.h \
 /usr/include/linux/if_ether.h linux-include/linux/if_arp.h \
 /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
 pcap.h pcap-namedb.h gencode.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
