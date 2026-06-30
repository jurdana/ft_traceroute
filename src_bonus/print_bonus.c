#include "../header/ft_traceroute_bonus.h"

void	print_help(int fd)
{
	dprintf(fd, "Usage:\n"
		"  ft_traceroute [ -n ] [ -f first_ttl ] [ -m max_ttl ] "
		"[ -p port ] [ -q nqueries ] host\n\n"
		"Options:\n"
		"  -f first_ttl  --first=first_ttl\n"
		"                              Start from the first_ttl hop (instead from 1)\n"
		"  -m max_ttl  --max-hops=max_ttl\n"
		"                              Set the max number of hops (max TTL to be\n"
		"                              reached). Default is 30\n"
		"  -n                          Do not resolve IP addresses to their domain names\n"
		"  -p port  --port=port        Set the destination port to use. It is the\n"
		"                              initial udp port value, incremented by each\n"
		"                              probe (default is 33434)\n"
		"  -q nqueries  --queries=nqueries\n"
		"                              Set the number of probes per each hop. Default is 3\n"
		"  --help                      Read this help and exit\n\n"
		"Arguments:\n"
		"      host          The host to traceroute to\n");
	exit(0);
}

void	print_header(t_trt *trt)
{
	char *ip_str;

	ip_str = inet_ntoa(trt->dest_addr.sin_addr);
	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
		trt->hostname, ip_str, trt->max_hops, trt->packet_size);
}
