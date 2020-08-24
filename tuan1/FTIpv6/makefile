all : send_file receive_file
.PHONY : all
send_file : client.c transfer.h
	gcc -Wall -O2 client.c -o client
receive_file : server.c transfer.h
	gcc -Wall -O2 server.c -o server
clean :
	rm client server