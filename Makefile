CC=gcc

EXE_NAME=capture

OBJ_DIR=.obj
OBJ_FILES=capture.o history.o dict.o

LIBS=-lpcap


.PHONY : all
all : $(OBJ_DIR) $(EXE_NAME)


$(EXE_NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $^ $(LIBS) -o $@

capturemake: capture.o history.o dict.o
	$(CC) -o $(EXE_NAME) $(addprefix $(OBJ_DIR), $^) $(LIBS)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $^ -o $@


history.o: history.c
	$(CC) -c $^ -o $(OBJ_DIR)$@

dict.o: dict.c
	$(CC) -c $^ -o $(OBJ_DIR)$@


$(OBJ_DIR):
	mkdir $@/


clean: 
	rm -f $(EXE_NAME)
	rm -rf $(OBJ_DIR)
	rm -f *.o
