all: lc4

lc4_memory.o: lc4_memory.c lc4_memory.h
	gcc -g -c lc4_memory.c

lc4_loader.o: lc4_loader.c lc4_loader.h
	gcc -g -c lc4_loader.c

lc4_disassembler.o: lc4_disassembler.c lc4_disassembler.h
	gcc -g -c lc4_disassembler.c

lc4: lc4.c lc4_memory.o lc4_loader.o lc4_disassembler.o
	gcc -g lc4.c lc4_memory.o lc4_loader.o lc4_disassembler.o -o lc4

clean:
	rm -rf *.o

clobber: clean
	rm -rf lc4
