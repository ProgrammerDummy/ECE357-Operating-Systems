CC = gcc
CFLAGS += -g

simplefind : main.c simplefind.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean

clean:
	@rm -rvf *.o

