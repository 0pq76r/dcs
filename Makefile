TARGET=dcs
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CC=gcc
CFLAGS=-Wall -g -lpthread

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET)
	
new: distclean all

install: all

