#include <pcap.h>
#include <stdio.h>

#define NUM_BYTES_PREDICTED 1024
#define RESIZE_THRESHOLD UINT_MAX

#define NUM_KEYS 7



/**
 * This struct handles the character occurences a single byte range
 * in the packet.
**/
typedef struct _list_entry {
	int value;			/* The ASCII value of the character */
	unsigned int occurences;	/* The number of occurences of this paricular character */
	struct _list_entry* next;	/* The ptr to the next struct, in an unsorted singly-linked list */
} list_entry;

/**
 * Struct to hold datapoints, which will be read by R UI interface
 */
typedef struct _point {
	int key;
	double freq;
} point;


point datapoints[NUM_KEYS];
pthread_mutex_t m;

struct _idx_info {
	list_entry* lists[NUM_BYTES_PREDICTED];		/* Stores an array of byte information for NUM_BYTES_PREDICTED bytes */
	unsigned int total_chars_counted[NUM_BYTES_PREDICTED];		
	unsigned int num_chars[NUM_BYTES_PREDICTED];
};

typedef struct _idx_info idx_info;

static idx_info charData;
static pcap_t* handle;


/** resolve raw domain name to easily recognizable domain name
 *  returns an int as opposed to a char*, to make communication with Rshiny easier
 */
int resolve_hostname(char*);


/** resolve IP address into raw domain name
 *  Takes raw IP address, and passes messy domain name to resolve_hostname
 */
int get_hostname(char*);


void setColor(float);
void resetColors(void);

void append_value(list_entry** entry, unsigned char c);
void thanosRow(int idx);

float run_char_analysis(int c, int idx);

//TODO rename
void pcap_callback(u_char* arg, const struct pcap_pkthdr* pkthdr, const u_char* packet);

void send_exit_signal(int signal);

//The interface for use as a library
void* startup(void* unused);
