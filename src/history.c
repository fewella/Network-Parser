#include "capture.h"
#include <stdlib.h>
#include <string.h>

void loadHistory(char* history_file) {
	if (!history_file) {
		history_file = "capture.history";
	}
	FILE* fp = fopen(history_file, "r");
	if (!fp) return;
	char buf[(255 * 5) + 2]; // 255 %c%d values + \n + \0
	size_t i;
	for (i = 0; i < NUM_BYTES_PREDICTED; ++i) {
		
		list_entry** entry_ptr = &charData.lists[i];

		unsigned int* total_ptr = &charData.total_chars_counted[i];
		*total_ptr = 0;

		unsigned int num_chars;
		//fscanf(fp, "%u ", &num_chars);
		fread(&num_chars, 4, 1, fp);
		charData.num_chars[i] = num_chars;
		//printf("num_chars = %u\n", num_chars);		
		unsigned int j;
		for (j = 0; j < num_chars; ++j) {
			
			*entry_ptr = malloc(sizeof(list_entry));
			//fscanf(fp, "%c%d ", &(*entry_ptr)->value, &(*entry_ptr)->occurences);
			fread(&(*entry_ptr)->value, 4, 1, fp);
			fread(&(*entry_ptr)->occurences, 4, 1, fp);
			//printf("We are at position %ld, with j = %u\n", ftell(fp), j);
		//	printf(" %c:%d", (*entry_ptr)->value, (*entry_ptr)->occurences);
			*total_ptr += (*entry_ptr)->occurences;
			entry_ptr = &(*entry_ptr)->next;
		}
		//printf("\n");
		*entry_ptr = NULL;
		fseek(fp, 4, SEEK_CUR); // Move past the newline character
	}
	fclose(fp);
}

void saveHistory(char* history_file) {
	if (!history_file) {
		history_file = "capture.history";
	}
	char buf[(255 * 5 * 4) + 4]; // 255 %c%d values + \n + \0
	int buf_itr = 0;
	FILE* fp = fopen(history_file, "w");
	size_t i;
	for (i = 0; i < NUM_BYTES_PREDICTED; ++i, buf_itr = 0) {
		list_entry* entry = charData.lists[i];
		memcpy(buf + buf_itr, &charData.num_chars[i], 4);
		buf_itr += 4;
		//printf("saving that we have %u unique characters:", charData.num_chars[i]);
		while (entry != NULL) {
			//printf(" %c:%d", entry->value, entry->occurences);
			memcpy(buf + buf_itr, &entry->value, 4);		buf_itr += 4;
			memcpy(buf + buf_itr, &entry->occurences, 4);		buf_itr += 4;
			entry = entry->next;
		}
		//printf("\n");
		int temp_val = '\n';
		memcpy(buf + buf_itr, &temp_val, 4);	buf_itr += 4;
		fwrite(buf, 1, buf_itr, fp); 
	}
	fclose(fp);
}
