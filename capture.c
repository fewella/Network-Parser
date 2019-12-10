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
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>

#include "history.h"
#include "capture.h"
#include "dict.h"

static int out_filedes;

static time_t prev;

int resolve_hostname(char* node) {
	char* prev = NULL;
	char* curr = strtok(node, ".");
	
	while (1) {
		char* p = strtok(NULL, ".");
		if (!p) {
			break;
		}
		prev = curr;
		curr = p;
	}

	if (!prev) {
		return -1;
	}

	size_t size = 1024;
	char* resolved = malloc(size);
	memset(resolved, '\0', size);

	strcat(resolved, prev);
	strcat(resolved, ".");
	strcat(resolved, curr);
	dprintf(out_filedes, "resolved: %s\n", resolved);

	

	// Mapping:
	if (!strcmp(resolved, "1e100.net")) {
		memset(resolved, '\0', size);
		strcat(resolved, "google.com");
	}

	int key = -1;
	
	if (!strcmp(resolved, "google.com")) {
		key = 0;
	} else if (!strcmp(resolved, "facebook.com")) {
		key = 1;
	} else if (!strcmp(resolved, "amazonaws.com")) {
		key = 2;
	} else if (!strcmp(resolved, "googleusercontent.com")) {
		key = 3;
	} else if (!strcmp(resolved, "illinois.edu")) {
		key = 4;
	} else if (!strcmp(resolved, "github.com")) {
		key = 5;
	} 

	free(resolved);

	return key;
}

int get_hostname(char* ip_address) {
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;

	char node[NI_MAXHOST];

	inet_pton(AF_INET, ip_address, &sa.sin_addr);
	int res = getnameinfo((struct sockaddr*)&sa, sizeof(sa), node, sizeof(node), NULL, 0, NI_NAMEREQD);
	if (res != 0) {
		dprintf(out_filedes, "getnameinfo failed!\n");
		return -1;
	}

	//printf("node: %s\n", node);
	return resolve_hostname(node);
}


void setColor(float value) {
	if (value >= 0.25) {
		dprintf(out_filedes, "\033[0;32m");
	}	
}


void resetColors() {
	dprintf(out_filedes, "\033[0m");
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
void pcap_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, 
        const u_char* packet) 
{ 

	unsigned size = 128;

    int i=0; 
    static int count=0; 

    struct ip *ip;
    struct tcphdr *tcp;
    ip = (struct ip*)(packet+sizeof(struct ether_header));
    tcp = (struct tcphdr*)(packet+sizeof(struct ether_header)+sizeof(struct ip));

    char src[size];
	int src_port = ntohs(tcp->th_sport);
	strcpy(src, inet_ntoa(ip->ip_src));
    dprintf(out_filedes, "Source Port %s:%d \n", src, src_port);
    
	char dst[size];
	int dst_port = ntohs(tcp->th_dport);
	strcpy(dst, inet_ntoa(ip->ip_dst));
    dprintf(out_filedes, "Dest Port %s:%d\n\n", dst, dst_port);

	int src_key = get_hostname(src);
	int dst_key = get_hostname(dst);
	
	insert(src_key, get(src_key) + pkthdr->len);
	insert(dst_key, get(dst_key) + pkthdr->len);

	dprintf(out_filedes, "src->total: %d -> %d\n", src_key, get(src_key));
	dprintf(out_filedes, "dst->total: %d -> %d\n", dst_key, get(dst_key));

	// check for 1 second interval. if so, make a struct for each entry in the dictionary and give to Rshiny
	double diff = time(NULL) - prev;
	printf("DIFFTIME: %f\n", diff);
	printf("time: %d\n", time(NULL));
	if (diff >= 1) {
	  printf("reset prev\n");
		dprintf(out_filedes, "ONE SECOND INTERVAL - DOING WLAK\n");
		prev = time(NULL);
		walk();
	}

	dprintf(out_filedes, "Packet Count: %d\n", ++count);    /* Number of Packets */
    dprintf(out_filedes, "Recieved Packet Size: %d\n", pkthdr->len);    /* Length of header */
    dprintf(out_filedes, "Payload:\n");                     /* And now the data */
	
	/*
	for(i=0;i<pkthdr->len;i++) { 
        float value = run_char_analysis(packet[i], (int) i);
		setColor(value);
		if(isprint(packet[i]))                // Check if the packet data is printable 
            dprintf(out_filedes, "%c",packet[i]);          // Print it 
        else
            dprintf(out_filedes, ".");          // If not print a . 
        if((i%16==0 && i!=0) || i==pkthdr->len-1) 
            dprintf(out_filedes, "\n"); 
		resetColors();
    }
*/
    dprintf(out_filedes, "\n\n");
}


void send_exit_signal(int signal) {
	dprintf(out_filedes, "Sending Exit Signal...\n");
	pcap_breakloop(handle);
}

void* startup(void* options_raw) {
  printf("STARTUP\n");
  pthread_mutex_init(&m, NULL);
  
  int i;
  for (i = 0; i < NUM_KEYS; i++) {
	point curr = datapoints[i];
	curr.key = i - 1;
	curr.freq = 0.0;
	datapoints[i] = curr;
  
	dprintf(out_filedes, "key, freq: %d, %f\n", curr.key, curr.freq);
  }
	
  long options = (long) options_raw;
  if (options == 1) {
    int devnull = open("/dev/null", O_WRONLY);
    out_filedes = devnull;
  } else {
    out_filedes = 1;
  }
  
  prev = time(NULL);
  
  for(i = 0; i < NUM_BYTES_PREDICTED; ++i) {
    charData.lists[i] = NULL;
    charData.total_chars_counted[i] = 0;
    charData.num_chars[i] = 0;
  }
  
  dprintf(out_filedes, "Loading history... \n"); //TODO implement
  // loadHistory(NULL);
  
  char error_buffer[PCAP_ERRBUF_SIZE];
  char *dev = "en0";
  int snapshot_len = 1028;
  int promiscuous = 1;
  int timeout = 100000;
  struct bpf_program filter;
  
  printf("1\n");
  
  char filter_exp[] = "";
  bpf_u_int32 subnet_mask, ip;
  printf("2\n");
  
  //dsignal(SIGINT, send_exit_signal);
  
  if (pcap_lookupnet(dev, &ip, &subnet_mask, error_buffer) == -1) {
    dprintf(out_filedes, "Could not get information for device: %s\n", dev);
    ip = 0;
    subnet_mask = 0;
  }
  handle = pcap_open_live(dev, BUFSIZ, 1, 1000, error_buffer);
  if (handle == NULL) {
    dprintf(out_filedes, "Could not open %s - %s\n", dev, error_buffer);
    return NULL;
  }
  if (pcap_compile(handle, &filter, filter_exp, 0, ip) == -1) {
    dprintf(out_filedes, "Bad filter - %s\n", pcap_geterr(handle));
    return NULL;
  }
  if (pcap_setfilter(handle, &filter) == -1) {
    dprintf(out_filedes, "Error setting filter - %s\n", pcap_geterr(handle));
    return NULL;
  }
  printf("3\n");
  printf("about to call callback\n");
  pcap_loop(handle, 0, pcap_callback, NULL);
  pcap_close(handle);
  
  dprintf(out_filedes, "Saving history...\n"); //TODO implement
  saveHistory(NULL);
  return NULL;
}

int main(int argc, char **argv) {
	startup(NULL);
  return 0;
}
