CC = clang
CFLAGS = -Wall -Werror -g

LD = clang
LDFLAGS = -Wall -Werror

CT = clang
CTFLAGS = -S

RM = rm

all: test

test: test.o
	$(LD) $(LDFLAGS) test.o -o test

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.c
	$(CT) $(CTFLAGS) -c $<

clean:
	$(RM) test *.o
