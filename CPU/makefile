exec=./cpuout.o
all:
	gcc -w -mavx2 -lpthread -o cpuout.o cpu.c
	$(exec) 1 1
	$(exec) 2 1
	$(exec) 4 1
	$(exec) 8 1
	$(exec) 1 2
	$(exec) 2 2
	$(exec) 4 2
	$(exec) 8 2
	$(exec) 1 3
	$(exec) 2 3
	$(exec) 4 3
	$(exec) 8 3
	$(exec) 1 4
	$(exec) 2 4
	$(exec) 4 4
	$(exec) 8 4
