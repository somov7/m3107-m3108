SRC = uint1024_t.c test.c
OBJS = ${SRC:.c=.o}

.c.o:
	gcc -c ${CFLAGS} $<

test: ${OBJS}
	gcc ${OBJS}

debug: ${OBJS}
	gcc -g -c ${CFLAGS} ${SRC}
	gcc -g ${OBJS}
