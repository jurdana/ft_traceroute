#ifndef FT_TRACEROUTE_BONUS_H
#define FT_TRACEROUTE_BONUS_H

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#define HOSTNAME_SIZE 256

typedef struct s_trt
{
	bool				reached;		// destination reached -> stop the loop
	bool				numeric;		// -n : print raw IPs, skip reverse DNS

	int		 			send_fd;		// UDP socket used to send probes
	int					recv_fd;		// raw ICMP socket used to read replies
	int					max_hops;		// -m : highest TTL to try (default 30)
	int					nprobes;		// -q : probes sent per hop (default 3)
	int					probes_sent;	// running count, drives the probe port
	int					initial_port;	// -p : base destination port (default 33434)
	int					packet_size;	// packet size shown in the banner
	int					ttl;			// current TTL (hop number)
	int					timeout;		// per-probe receive timeout, in seconds
	int					ip_shown;		// has an IP already been printed on this line
	int					first;			// is this the first item printed on the line
	int					replied;		// did the current probe get a reply (1/0)
	int					expected_port;	// dest port awaited for the current probe
	int					first_ttl;		// -f : starting TTL (default 1)
	int     			host_index;		// host's argv index, for the error message

	double				rtt;			// current probe round-trip time, in ms

	struct in_addr		hop_ip;			// IP of the router that replied
	struct in_addr		last_ip;		// last IP printed on this line

	struct timeval		start;			// current probe send timestamp

	struct sockaddr_in	dest_addr;		// resolved destination (IP + per-probe port)

	char				hostname[HOSTNAME_SIZE];	// target as given on the command line
}	t_trt;

// INIT.C
void	init_hostname(t_trt *trt);
void	init(t_trt *trt);
void	init_socket(t_trt *trt);

// PARSE.C
bool	is_number(char *str);
int		parse_number(char *optarg, int opt);
void	parse_args(int argc, char **argv, t_trt *trt);

// PRINT.C
void	print_help(int fd);
void	print_header(t_trt *trt);

// UTILS.C
int		ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

// PROBE.C
void	print_addr(t_trt *trt);
void	print_result(t_trt *trt);
void	send_one(t_trt *trt);
void	sending_probe(t_trt *trt);
int		receiving_probe(t_trt *trt);

#endif