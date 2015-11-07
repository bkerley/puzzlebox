CC = clang
CFLAGS = -Wall -Werror -g -fblocks

LD = clang
LDFLAGS = -Wall -Werror

CT = clang
CTFLAGS = -S

RM = rm

all: test

test: test.o
	$(LD) $(LDFLAGS) test.o -o test

http_notice: http_notice.o llist.o
	$(LD) $(LDFLAGS) $< llist.o -o http_notice

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.c
	$(CT) $(CTFLAGS) -c $<

clean:
	$(RM) test *.o
