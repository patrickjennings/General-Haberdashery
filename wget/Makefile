CC = gcc
CFLAGS = -g -Wall -Werror -W -Wextra
LIBS = -lcurl
PROJ = wget
target = $(PROJ)

OBJS = wget.o
.SUFFIXES: .o .c

$(PROJ): $(OBJS)
	$(CC) -o $(PROJ) $(CFLAGS) $(LIBS) $^ 

.c .o:
	$(CC) $(CFLAGS) $(LIBS) -c $<

clean:
	rm -rf $(OBJS) $(PROJ)
