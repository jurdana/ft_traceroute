#include "../header/ft_traceroute.h"

void	print_help(int fd)
{
	dprintf(fd, "Usage:\n"
		"  ft_traceroute [ --help ] host\n\n"
		"Options:\n"
		"  --help                      Read this help and exit\n\n"
		"Arguments:\n"
		"      host          the host to traceroute to\n");
	exit(0);
}

void	print_header(t_trt *trt)
{
	char *ip_str;

	ip_str = inet_ntoa(trt->dest_addr.sin_addr);

	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
		trt->hostname, ip_str, trt->max_hops, trt->packet_size);
}