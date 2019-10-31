CC=gcc

EXE_NAME=capture

OBJ_DIR=.obj/

LIBS=-lpcap

.PHONY : all
all : $(OBJ_DIR) capturemake

capturemake: capture.o history.o 
	$(CC) -o $(EXE_NAME) $(addprefix $(OBJ_DIR), $^) $(LIBS)

capture.o: capture.c
	$(CC) -c $^ -o $(OBJ_DIR)$@

history.o: history.c
	$(CC) -c $^ -o $(OBJ_DIR)$@
$(OBJ_DIR):
	mkdir .obj/

clean: 
	rm $(EXE_NAME)
	rm -rf $(OBJ_DIR)
