CC = gcc
CFLAGS = -Wall -Werror -Wextra

all:
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -rf server client

fclean: clean
	
re: fclean all