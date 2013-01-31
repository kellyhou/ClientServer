all: dict.o input.o ansi_code.o glist.o
	gcc -o dict $^ -g

dict.o: dict.c
	gcc -c dict.c -g

input.o: input.c input.h
	gcc -c input.c -g
ansi_code.o: ansi_code.c ansi_code.h
	gcc -c ansi_code.c -g

glist.o: glist.c glist.h
	gcc -c glist.c -g

clean:
	rm -rf dict.o input.o ansi_code.o glist.o dict
