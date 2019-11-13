#include <pcap.h>

#define NUM_BYTES_PREDICTED 1024
#define RESIZE_THRESHOLD UINT_MAX

/**
 * This struct handles the character occurences a single byte range
 * in the packet.
**/
struct _list_entry {
	int value;			/* The ASCII value of the character */
	unsigned int occurences;	/* The number of occurences of this paricular character */
	struct _list_entry* next;	/* The ptr to the next struct, in an unsorted singly-linked list */
};

typedef struct _list_entry list_entry;

struct _idx_info {
	list_entry* lists[NUM_BYTES_PREDICTED];				/* Stores an array of byte information for NUM_BYTES_PREDICTED bytes */
	unsigned int total_chars_counted[NUM_BYTES_PREDICTED];		
	unsigned int num_chars[NUM_BYTES_PREDICTED];
};

typedef struct _idx_info idx_info;

static idx_info charData;

void setColor(float);
void resetColors(void);

void append_value(list_entry** entry, unsigned char c);
void thanosRow(int idx);

float run_char_analysis(int c, int idx);

//TODO rename
void pcap_callback(u_char* arg, const struct pcap_pkthdr* pkthdr, const u_char* packet);

void send_exit_signal(int signal);

