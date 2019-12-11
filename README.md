# Network-Parser

## What
We would like to build a program to monitor what your computer is sending and receiving, and displaying it to the user in an extremely readable way

## Why
When you’re using the Internet, your computer is constantly sending and receiving information. For instance, if you’re using a website to message someone else, every time you send or receive a message your computer has to communicate with that website. Ideally, your computer only communicates your messages and whatever necessary information might be needed to maintain the connection. However, many companies are known for obtaining data with questionable methods, including asking your computer for data you don’t necessarily know is being sent. Maybe the website is recording your mouse movement and scrolling to determine how long you spend reading a certain section. Maybe the website saves what you’re typing before you even press enter. 


While there are already apps which monitor network traffic, many of them do very little purging on their own, and display everything that the network packets contain, which is not very helpful for determining what data websites might be collecting from your computer. 

## How
We used a library called [libpcap](https://github.com/the-tcpdump-group/libpcap) to do most of the packet analysis.

We used [Rshiny](https://shiny.rstudio.com/) to create the visualisation.

## How to

To run just the packet capture script, only ```libpcap``` is needed:

```bash
cd src/
make && ./capture
```

To run the interface, Rshiny is needed as well:

```bash
make interface && ./runShiny.sh
```

## Future Additions
* Stratify data by port number/protocol
* Intrusion detection

## Who and When (and actually Why)
* Ajay Fewell - fewella2 at uiuc dot edu
* Rishub Podar - podar2 at uiuc dot edu
* Drew Litkowiak - dnl2 at uiuc dot edu
* Mentored by Aniket Shirke - anikets at illinois dot edu
Fall 2019, CS296-41
