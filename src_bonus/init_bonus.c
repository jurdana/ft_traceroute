#include "../header/ft_traceroute_bonus.h"

/*
Resolves the target hostname/IP into an IPv4 sockaddr (AF_INET only).
On failure, prints the resolver error plus the reference "Cannot handle"
line and exits 2. On success, stores the address in trt->dest_addr.
*/
void	init_hostname(t_trt *trt)
{
	int				status;
	struct addrinfo	*res;
	struct addrinfo hints = {0};

	hints.ai_family = AF_INET;
	status = getaddrinfo(trt->hostname, NULL, &hints, &res);
	if (status != 0)
	{
		dprintf(2, "%s: %s\n", trt->hostname, gai_strerror(status));
		dprintf(2, "Cannot handle \"host\" cmdline arg `%s' on position 1 (argc %d)\n",
			trt->hostname, trt->host_index);
		exit (2);
	}

	trt->dest_addr = *((struct sockaddr_in *)res->ai_addr);
	
	freeaddrinfo(res);
}

/*
Sets all default option values before argument parsing:
30 max hops, 60-byte packets, 3 probes/hop, 1s timeout,
base port 33434, first TTL 1, DNS resolution on.
*/
void	init(t_trt *trt)
{
	trt->max_hops = 30;
	trt->packet_size = 60;
	trt->nprobes = 3;
	trt->timeout = 1;
	trt->initial_port = 33434;
	trt->first_ttl = 1;
	trt->numeric = false;
}

/*
Opens the two sockets: a UDP socket to send probes and a raw ICMP
socket to receive the replies, then sets the receive timeout
(SO_RCVTIMEO). Exits 1 on any failure (raw socket needs root).
*/
void	init_socket(t_trt *trt)
{
	struct timeval tv;

	trt->send_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (trt->send_fd < 0)
	{
		dprintf(2, "ft_traceroute: socket: %s\n", strerror(errno));
		exit (1);
	}

	trt->recv_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (trt->recv_fd < 0)
	{
		dprintf(2, "ft_traceroute: socket: %s\n", strerror(errno));
		close(trt->send_fd);
		exit (1);
	}

	tv.tv_sec = trt->timeout;
	tv.tv_usec = 0;
	if(setsockopt(trt->recv_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
	{
		dprintf(2, "ft_traceroute: setsockopt: %s\n", strerror(errno));
		exit (1);
	}
}
