CC = gcc
CFLAGS = -O0

all: main

OBJS := main.o quick.o quick_mid.o

deps := $(OBJS:%.o=.%.o.d)

main: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

test: main
	@./main

clean:
	rm -f $(OBJS) $(deps) *~ main
	rm -rf *.dSYM

-include $(deps)