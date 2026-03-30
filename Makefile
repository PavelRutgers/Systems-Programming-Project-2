CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
LDFLAGS = -lm

OBJS = compare.o wfd.o file_set.o walker.o jsd.o

compare: $(OBJS)
	$(CC) $(CFLAGS) -o compare $(OBJS) $(LDFLAGS)

compare.o: compare.c file_set.h walker.h jsd.h
wfd.o: wfd.c wfd.h
file_set.o: file_set.c file_set.h wfd.h
walker.o: walker.c walker.h file_set.h
jsd.o: jsd.c jsd.h file_set.h wfd.h

clean:
	rm -f $(OBJS) compare