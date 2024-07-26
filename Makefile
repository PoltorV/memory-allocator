CFLAGS=-Wall -Wextra -pedantic -g

heap: main.c
	gcc  $(CFLAGS) -o heap main.c