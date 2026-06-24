#include "../header/ft_traceroute.h"

void	init_hostname(t_trt *trt)
{
	int				status;
	struct addrinfo	*res;
	struct addrinfo hints = {0};
	// char			*ip_str;
	// char			ip_str[INET_ADDRSTRLEN];
	
	// memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	status = getaddrinfo(trt->hostname, NULL, &hints, &res);
	if (status != 0)
	{
		dprintf(2, "%s: %s\n", trt->hostname, gai_strerror(status));
		exit (2);
	}

	trt->dest_addr = *((struct sockaddr_in *)res->ai_addr);
	
	freeaddrinfo(res);

	// ip_str = inet_ntoa(trt->dest_addr.sin_addr);

	// printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
	// 	trt->hostname, ip_str, trt->max_hops, trt->packet_size);
}

void	init(t_trt *trt)
{
	trt->max_hops = 30;
	trt->packet_size = 60;
	trt->nprobes = 3;
	trt->timeout = 3;
	trt->initial_port = 33434;
}

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