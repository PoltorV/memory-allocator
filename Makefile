CFLAGS=-Wall -Wextra -Werror -pedantic -g

heap: main.c
	gcc  $(CFLAGS) -o heap main.c