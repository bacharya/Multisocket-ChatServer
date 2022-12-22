CFLAGS = -Wall -g
#LDLIBS = -lcrypto -lssl

client: chat_client.o utils.h chat_client.c
	cc -o client chat_client.o

server: chat_server.o utils.h chat_server.c
	cc -o server chat_server.o

clean:
	rm  chat_server.o chat_client.o client server
