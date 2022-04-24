exec = Jolang
source = $(wildcard src/*.c)
objects = $(source:.c=.o)
flags = -g -Wall

$(exec): $(objects)
	gcc $(flags) $^ -o $@

%.o: %.c
	gcc -c $(flags) $< -o $@

clean:
	-rm src/*.o
	-rm *.o
	-rm $(exec)
	-rm out
	-rm *.asm

runASM:
	nasm -f elf32 out.asm -o out.o
	ld -m elf_i386 out.o -o out
	./out
