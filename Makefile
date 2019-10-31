CC=gcc

EXE_NAME=capture

OBJ_DIR=.obj/

LIBS=-lpcap

.PHONY : all
all : capturemake

capturemake: capture.o history.o 
	$(CC) -o $(EXE_NAME) $(addprefix $(OBJ_DIR), $^) $(LIBS)

capture.o: capture.c
	$(CC) -c $^ -o $(OBJ_DIR)$@

history.o: history.c
	$(CC) -c $^ -o $(OBJ_DIR)$@

clean: 
	rm $(EXE_NAME)
	rm -rf $(OBJ_DIR)
