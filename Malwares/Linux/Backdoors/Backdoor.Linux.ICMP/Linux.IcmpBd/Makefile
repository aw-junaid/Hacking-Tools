CC = cc
FLAGS = -O2 -Wall
SERVER = server.o sendicmp.o wrapper.o opencall.o
CLIENT = client.o sendicmp.o wrapper.o

default:	
		@echo 'server' \|\| 'client' \|\| 'clean'
client:		${CLIENT}
		${CC} ${FLAGS} -o ibd-client ${CLIENT}
server: 	${SERVER}
		${CC} ${FLAGS} -o ibd-server ${SERVER}
clean:
		rm -f *.o
