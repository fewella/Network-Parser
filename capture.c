#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <time.h>
#define ERR_BUF_SIZE 2048

#include "includes/dictionary.h"

/**
 * Sources:
 * 	http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
 *  https://www.tcpdump.org/pcap.html
 *
 */

int main() {
	char* dev, errbuf[ERR_BUF_SIZE];
	int packet_delay_ms = 1;

	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		exit(2);
	}
	printf("Device: %s\n", dev);

	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, packet_delay_ms, errbuf);

	if (handle == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		exit(2);
	}
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return(2);
	}

	struct bpf_program fp;		/* The compiled filter */
	char filter_exp[] = "len >= 0";	/* https://www.tcpdump.org/manpages/pcap-filter.7.html */
	bpf_u_int32 mask;		/* Our netmask */
	bpf_u_int32 net;		/* Our IP */
	struct pcap_pkthdr header;	/* The header that pcap gives us */
	const u_char *packet;		/* The actual packet */
		
	/* Compile and apply the filter */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return(2);
	}
	
	/* Grab a packet */
	dictionary* ip_to_freq;
	time_t start = time(NULL);
	size_t size = 0;
	while (1) {
		packet = pcap_next(handle, &header);
		size += header.len;
		// flush to stdout every second
		time_t curr = time(NULL);
		if ((size_t)(curr - start) >= 1) {
			printf("Traffic size: [%zu]\n", size);
			start = curr;
			size = 0;
		}
	}
	/* And close the session */
	pcap_close(handle);
	return(0);
}

