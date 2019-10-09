CC=gcc

capturemake: 
	$(CC) -o capture capture.c


clean: 
	rm capture
