#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR_BUF_SIZE 2048

/**
 * Sources:
 * 	http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
 *  https://www.tcpdump.org/pcap.html
 *
 */

int main() {
	char* dev, errbuf[ERR_BUF_SIZE];

	dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		exit(2);
	}
	printf("Device: %s\n", dev);

	pcap_t* handle = pcap_open_live(dev, BUFSIZ /* Defined in <pcap.h> */, 1, 1000, errbuf);

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
	while (1) {
		packet = pcap_next(handle, &header);
		/* Print its length */
		printf("Jacked a packet with length of [%d]\n", header.len);
	}
	/* And close the session */
	pcap_close(handle);
	return(0);
}
