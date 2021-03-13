CC = gcc
CFLAGS = -g -std=gnu11 -Wall
SRCHFILE = webSearch.h
OBJS = indexPage.o crawler.o searchQuery.o

all: webSearch
webSearch: webSearch.c $(OBJS) $(SRCHFILE)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $< -lm
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o indexPage.exe