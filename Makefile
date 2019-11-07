CC=gcc

EXE_NAME=capture

OBJ_DIR=.obj
OBJ_FILES=capture.o history.o dict.o

LIBS=-lpcap


.PHONY : all
all : $(OBJ_DIR) $(EXE_NAME)


$(EXE_NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $^ $(LIBS) -o $@


$(OBJ_DIR)/%.o: %.c
	$(CC) -c $^ -o $@


$(OBJ_DIR):
	mkdir $@/


clean: 
	rm -f $(EXE_NAME)
	rm -rf $(OBJ_DIR)
