CFLAGS=-Wall -Wextra -Werror -pedantic -g

heap: main.c heap.h heap.c
	gcc  $(CFLAGS) -o heap main.c heap.c heap.h