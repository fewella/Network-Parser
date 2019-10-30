CC=gcc

LIBS=-lpcap
INC = -I./includes/

.PHONY : all
all : capturemake

capturemake: 
	$(CC) -o capture capture.c $(LIBS) $(INC)


clean: 
	rm capture
