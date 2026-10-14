#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdio.h>
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

#define HOSTNAME_SIZE 256

typedef struct s_trt
{
	bool				reached;

	int		 			send_fd;
	int					recv_fd;
	int					max_hops;
	int					nprobes;
	int					probes_sent;
	int					initial_port;
	int					packet_size; //payload
	int					ttl;
	int					timeout;
	int					ip_shown;
	int					first;
	int					replied;
	int					expected_port;
	int     			host_index;
	
	double				rtt;

	struct in_addr		hop_ip;
	struct in_addr		last_ip;

	struct timeval		start;

	struct sockaddr_in	dest_addr;

	char				hostname[HOSTNAME_SIZE];
}	t_trt;

// INIT.C
void	init_hostname(t_trt *trt);
void	init(t_trt *trt);
void	init_socket(t_trt *trt);

// PARSE.C
void	parse_args(int argc, char **argv, t_trt *trt);

// PRINT.C
void	print_help(int fd);
void	print_header(t_trt *trt);

// UTILS.C
int	ft_strcmp(const char *s1, const char *s2);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

// PROBE.C
void	print_result(t_trt *trt);
void	send_one(t_trt *trt);
void	sending_probe(t_trt *trt);
int		receiving_probe(t_trt *trt);

#endif