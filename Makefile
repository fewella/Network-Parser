CC=gcc

EXE_NAME=capture

LIBS=-lpcap

.PHONY : all
all : capturemake

capturemake: capture.c 
	$(CC) -o $(EXE_NAME) $< $(LIBS)


clean: 
	rm $(EXE_NAME)
