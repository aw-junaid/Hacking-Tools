# Sniffit Makefile -- Brecht Claerhout
#
# Parts that could need adjusting are commented
#
#Don't change this
.PHONY : clean
.PHONY : config
.PHONY : no_arguments
.PHONY : all
.PHONY : sniffit

#This is executed when just typing Make, don't change

no_arguments:
	@echo 'Please read the README.* and IMPORTANT files'
	@echo 'use: make system'
	@echo 'where system can be: linux, sunos, irix, freebsd'
	@echo '(sunos: compiles for SunOS/Solaris)'
	@echo '(freebsd: first make the patch!!!!! read IMPORTANT)' 

sniffit:
	@echo 'Please read the README.* and IMPORTANT files'
	@echo 'use: make system'
	@echo 'where system can be: linux, sunos, irix, freebsd'
	@echo '(sunos: compiles for SunOS/Solaris)'
	@echo '(freebsd: first make the patch!!!!! read IMPORTANT)' 
all:
	@echo 'Please read the README.* and IMPORTANT files'
	@echo 'use: make system'
	@echo 'where system can be: linux, sunos, irix, freebsd'
	@echo '(sunos: compiles for SunOS/Solaris)'
	@echo '(freebsd: first make the patch!!!!! read IMPORTANT)' 

#Some Vars (you could change if you know what y'r doing)
CC         = gcc
SNIFFIT    = sniffit.0.3.3.c
DEP_FILES  = config.h sn_packets.h pcap.h sn_data.h sn_defines.h \
             sn_interface.h sn_oldether.h sn_cfgfile.h sn_logfile.h
GEN_FLAG   = -w -O2 -o sniffit
GEN_OPT    = -I./libpcap -L./libpcap -lpcap
#GEN_OPT    = -I./libpcap -L./libpcap -lpcap -DDEBUG

#Clean up everthing, don't change 
clean:
	cd libpcap; make clean; cd ..
	rm -f *.o sniffit

#Config the libpcap, don't change 
config:
	cd libpcap; ./configure; cd ..
	@echo 'You can now "make system"'

################################### Linux ####################################
# Remove this line if you want to exclude the INTERACTIVE PART (see config.h)
SYSTEM_OPT_LINUX := -DLINUX -USUNOS -UIRIX -UFREEBSD -UBSDI -I/usr/include/ncurses -L/usr/include/ncurses -lncurses
# Use this line instead if excluding the INTERACTIVE PART
# SYSTEM_OPT_LINUX := -DLINUX -USUNOS -UIRIX -UFREEBSD -UBSDI
 
linux:	$(SNIFFIT) $(DEP_FILES)
	cd libpcap; ./configure; make; cd ..
	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) $(SYSTEM_OPT_LINUX) 
	strip sniffit

############################### SunOS/Solaris ################################
# Remove this line if you want to exclude the INTERACTIVE PART (see config.h)
SYSTEM_OPT_SUNOS := -ULINUX -DSUNOS -UIRIX -UFREEBSD -UBSDI -lsocket -lnsl -lncurses 
# Use this line instead if excluding the INTERACTIVE PART
# SYSTEM_OPT_SUNOS := -ULINUX -DSUNOS -UIRIX -UFREEBSD -UBSDI -lsocket -lnsl

# If SunOS/Solaris compile doesn't work, try this removing '-lsocket' in the
# previous lines....

# It could be necessary to specify the path for the ncurses.h file.

sunos:	$(SNIFFIT) $(DEP_FILES) 
	cd libpcap; ./configure; make; cd .. 
	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) $(SYSTEM_OPT_SUNOS) 
	strip sniffit

#################################### IRIX ####################################
SYSTEM_OPT_IRIX := -ULINUX -USUNOS -DIRIX -UFREEBSD -UBSDI 

# It could be necessary to specify the path for the ncurses.h file.

irix:	$(SNIFFIT) $(DEP_FILES) 
	cd libpcap; ./configure; make; cd .. 
	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) $(SYSTEM_OPT_IRIX) 
	strip sniffit

################################# FreeBSD ####################################
# Remove this line if you want to exclude the INTERACTIVE PART (see config.h)
SYSTEM_OPT_FREEBSD = -ULINUX -USUNOS -UIRIX -UBSDI -DFREEBSD -lncurses  
# Use this line instead if excluding the INTERACTIVE PART
# SYSTEM_OPT_FREEBSD = -ULINUX -USUNOS -UIRIX -UBSDI -DFREEBSD

# It could be necessary to specify the path for the ncurses.h file.

freebsd:$(SNIFFIT) $(DEP_FILES) 
	cd libpcap; ./configure; make; cd ..
	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) $(SYSTEM_OPT_FREEBSD)
	strip sniffit

# BSDI  -- Later development
#SYSTEM_OPT_BSDI := -ULINUX -USUNOS -UIRIX -UFREEBSD -DBSDI
#bsdi:	$(SNIFFIT) $(DEP_FILES)
#	cd libpcap; ./configure; make; cd .. 
#	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) $(SYSTEM_OPT_BSDI) 
#	strip sniffit


#adjust this section for experimental compiling
sniffit_manual: $(SNIFFIT) $(DEP_FILES)
	cd libpcap; ./configure; make; cd ..
#add y'r flags to this line...
	$(CC) $(GEN_FLAG) $(SNIFFIT) $(GEN_OPT) 

