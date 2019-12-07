CC=gcc

EXE_NAME=capture
INTERFACE=interface.so


OBJ_DIR=.obj
OBJ_FILES=capture.o history.o dict.o
OBJ_FILES_EXT= $(OBJ_FILES) r_to_c_interface.o

LIBS=-lpcap -lpthread
INT_LIBS = -lpthread

.PHONY: all library interface
all: $(OBJ_DIR) $(EXE_NAME)

library: $(OBJ_DIR) $(EXE_NAME).so

interface: $(OBJ_DIR) $(INTERFACE)

$(INTERFACE): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES_EXT))
	R CMD SHLIB $^ $(LIBS) $(INT_LIBS) -o $@

$(EXE_NAME).so: $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) -shared $^ $(LIBS) -o $@

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
	rm -f *.o *.so
