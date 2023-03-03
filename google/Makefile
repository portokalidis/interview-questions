CFLAGS=-O2 -Wall
PROGS=decompression

.PHONY: all clean

all:  $(PROGS)

$(PROGS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm *.o $(PROGS)
