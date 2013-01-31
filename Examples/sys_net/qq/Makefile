all: server.o  gen_single_nohead_pre.o client.o ansi_code.o
	gcc -o server server.o ansi_code.o gen_single_nohead_pre.o
	gcc -o client client.o ansi_code.o gen_single_nohead_pre.o

server.o: server.c tcp.h gen_single_nohead_pre.h
	gcc -c -o server.o server.c

client.o: client.c tcp.h gen_single_nohead_pre.h
	gcc -c -o client.o client.c

gen_single_nohead_pre.o: gen_single_nohead_pre.c gen_single_nohead_pre.h
	gcc -c -o gen_single_nohead_pre.o gen_single_nohead_pre.c

ansi_code.o: ansi_code.c ansi_code.h
	gcc -c -o ansi_code.o ansi_code.c 

clean:
	rm -rf *.o server client 

		
	
