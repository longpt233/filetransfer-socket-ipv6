all : send_file receive_file
.PHONY : all
send_file : client.c transfer.h
	gcc -Wall client.c -o client
receive_file : server.c transfer.h
	gcc -Wall server.c -o server
clean :
	rm client server