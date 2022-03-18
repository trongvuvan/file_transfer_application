CFLAGS = -c -Wall
CC = gcc -g
LIBS =  -lm 

all: server client

server: file.o user.o folder.o group.o show.o menu.o server.o 
	${CC} server.o file.o user.o folder.o group.o show.o menu.o -o server -lmysqlclient -lpthread
client: file.o user.o folder.o group.o show.o menu.o client.o
	${CC} client.o file.o folder.o user.o group.o show.o menu.o -o client -lmysqlclient -lpthread
server.o: server.c
	${CC} ${CFLAGS} -lmysqlclient -lpthread server.c 
client.o: client.c
	${CC} ${CFLAGS} -lmysqlclient -lpthread client.c
file.o: file.c
	${CC} ${CFLAGS} -lmysqlclient file.c
user.o: user.c
	${CC} ${CFLAGS} -lmysqlclient user.c
folder.o: folder.c
	${CC} ${CFLAGS} folder.c
group.o: group.c
	${CC} ${CFLAGS} group.c
show.o: show.c
	${CC} ${CFLAGS} show.c
menu.o: menu.c
	${CC} ${CFLAGS} menu.c
clean:
	rm -f *.o *~


