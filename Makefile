battleships: main.o client.o server.o
	cc -o battleships client.c server.c main.c

main.o : main.c configuration.h
	cc -c main.c

client.o : client.c configuration.h
	cc -c client.c

server.o : server.c configuration.h
	cc -c server.c

clean :
	rm battleships main.o client.o server.o
