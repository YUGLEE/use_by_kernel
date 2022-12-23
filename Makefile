all: client service
client: client.c
	gcc client.c -static -o client

service: service.c
	gcc service.c -static -o service