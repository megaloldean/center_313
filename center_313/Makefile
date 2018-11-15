all:
	gcc center313.c -c -o center313.o
	gcc read.c -c -o read.o
	gcc center313.o read.o -o test.elf
clean:
	rm center313.o -f
	rm read.o -f
	rm test.elf
	