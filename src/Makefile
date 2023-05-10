CC=gcc
CFLAGS= -g

mysh: shell.c interpreter.c shellmemory.c helper.c policies.c
	$(CC) $(CFLAGS) -D FRAME_STORE_SIZE=$(framesize) -D VAR_STORE_SIZE=$(varmemsize) -c shell.c interpreter.c shellmemory.c helper.c policies.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o helper.o policies.o

clean: 
	rm mysh; rm *.o