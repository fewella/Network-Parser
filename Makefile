CC=gcc

LIBS=-lpcap

.PHONY : all
all : capturemake

capturemake: 
	$(CC) -o capture capture.c $(LIBS)


clean: 
	rm capture
