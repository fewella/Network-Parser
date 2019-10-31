#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <ctype.h>
#include <pthread.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "history.h"
#include "capture.h"


static pcap_t* handle;

void setColor(float value) {
	if (value >= 0.25) {
		printf("\033[0;32m");
	}	
}


void resetColors() {
	printf("\033[0m");
}

void append_value(list_entry** entry, unsigned char c) {	
	*entry = malloc(sizeof(list_entry));
	**entry = (list_entry) { c, 1, NULL };
}

void thanosRow(int idx) {
	list_entry** entry = &charData.lists[idx];
	while (*entry != NULL) {
		(*entry)->occurences /= 2;
		if ((*entry)->occurences == 0) {
			list_entry* temp = *entry;
			*entry = (*entry)->next;
			free(temp);
			--charData.num_chars[idx];
		} else {
			*entry = (*entry)->next;
		}
	}
}

float run_char_analysis(int c, int idx) {
	if (idx < 0 || idx >= NUM_BYTES_PREDICTED) return 0;
	list_entry** entry = &charData.lists[idx];
	if (*entry == NULL) {
		append_value(entry, c);
		unsigned int total_sum = ++charData.total_chars_counted[idx];
		unsigned int total_unique = ++charData.num_chars[idx];
		float ret_value = ((float) total_unique) / ((float) total_sum);
		if (total_sum == RESIZE_THRESHOLD) {
			thanosRow(idx);
		}
		return ret_value;
	}
	bool exists = 0;
	while (*entry != NULL) {
		if ((*entry)->value == c) {
			exists = 1;
			(*entry)->occurences++;
			break;
		}
		entry = &(*entry)->next;
	}

	unsigned int total_sum, total_unique;
	total_sum = ++charData.total_chars_counted[idx];
	
	if (exists == 0) {
		append_value(entry, c);
		total_unique = ++charData.num_chars[idx];
	} else {
		total_unique = charData.num_chars[idx];
	}
	
	float ret_value = ((float) total_unique * (*entry)->occurences) / ((float) total_sum);
	if (total_sum == RESIZE_THRESHOLD) {
		thanosRow(idx);
	}
	return ret_value;

}

/* This function can be used as a callback for pcap_loop() */
void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, 
        const u_char* packet) 
{ 
    int i=0; 
    static int count=0; 
 
    printf("Packet Count: %d\n", ++count);    /* Number of Packets */
    printf("Recieved Packet Size: %d\n", pkthdr->len);    /* Length of header */
    printf("Payload:\n");                     /* And now the data */
    for(i=0;i<pkthdr->len;i++) { 
        float value = run_char_analysis(packet[i], (int) i);
		setColor(value);
		if(isprint(packet[i]))                /* Check if the packet data is printable */
            printf("%c",packet[i]);          /* Print it */
        else
            printf(".");          /* If not print a . */
        if((i%16==0 && i!=0) || i==pkthdr->len-1) 
            printf("\n"); 
		resetColors();
    }
}

void send_exit_signal(int signal) {
	printf("Sending Exit Signal...\n");
	pcap_breakloop(handle);
}

int main(int argc, char **argv) {
	int i;
	for(i = 0; i < NUM_BYTES_PREDICTED; ++i) {
		charData.lists[i] = NULL;
		charData.total_chars_counted[i] = 0;
		charData.num_chars[i] = 0;
	}
	
	printf("Loading history... \n"); //TODO implement
	loadHistory(NULL);
	
    char error_buffer[PCAP_ERRBUF_SIZE];
    char *dev = "en0";
    int snapshot_len = 1028;
    int promiscuous = 1;
    int timeout = 100000;
    struct bpf_program filter;

    char filter_exp[] = "";
    bpf_u_int32 subnet_mask, ip;

	signal(SIGINT, send_exit_signal);

    if (pcap_lookupnet(dev, &ip, &subnet_mask, error_buffer) == -1) {
        printf("Could not get information for device: %s\n", dev);
        ip = 0;
        subnet_mask = 0;
    }
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, error_buffer);
    if (handle == NULL) {
        printf("Could not open %s - %s\n", dev, error_buffer);
        return 2;
    }
    if (pcap_compile(handle, &filter, filter_exp, 0, ip) == -1) {
        printf("Bad filter - %s\n", pcap_geterr(handle));
        return 2;
    }
    if (pcap_setfilter(handle, &filter) == -1) {
        printf("Error setting filter - %s\n", pcap_geterr(handle));
        return 2;
    }

    pcap_loop(handle, 0, another_callback, NULL);
    pcap_close(handle);

	printf("Saving history...\n"); //TODO implement
    	saveHistory(NULL);
	

	return 0;
}
