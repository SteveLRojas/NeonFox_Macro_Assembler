CFLAGS=-std=c99 -pedantic -Wall -Wextra -Wconversion -D_XOPEN_SOURCE -march=native -O3 -flto
LDLIBS=-lm -flto

.PHONY: all

all: nfmasm.exe

nfmasm.exe: NeonFox_Macro_Assembler.o

%.exe:
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

lib%.a: %.o
	$(AR) -rs $@ $^

.PHONY: clean
clean:
	$(RM) -- *.exe *.a *.o
