#include "../header/ft_traceroute.h"

void	sending_probe(t_trt *trt)
{

	struct in_addr hop_ip;
	struct in_addr last_ip;
	struct	timeval start;
	char	payload[32] = {0};
	double	rtt;
	int		replied;
	int		ip_shown;
	int		first;


	trt->ttl = 1;
	trt->dest_addr.sin_port = htons(trt->initial_port);


	while (trt->ttl <= trt->max_hops)
	{

		if (setsockopt(trt->send_fd, IPPROTO_IP, IP_TTL, &trt->ttl, sizeof(trt->ttl)))
		{
			dprintf(2, "ft_traceroute: setsockopt failure");
			exit (1);
		}

		printf("%2d", trt->ttl);
		ip_shown = 0;
		first = 1;
		for (int i = 0; i < 3; i++)
		{
		
			gettimeofday(&start, NULL);
			trt->dest_addr.sin_port = htons(trt->initial_port + trt->probes_sent);

			if (sendto(trt->send_fd, payload, sizeof(payload), 0,
				(struct sockaddr *)&trt->dest_addr, sizeof(trt->dest_addr)) < 0)
			{
				dprintf(2, "ft_traceroute: sendto: %s\n", strerror(errno));
				exit(1);
			}
			replied = receiving_probe(trt, &start, &hop_ip, &rtt);
			trt->probes_sent++;
			if (replied)
			{
				if (!ip_shown || hop_ip.s_addr != last_ip.s_addr)
				{
					if (first)
						printf("  %s", inet_ntoa(hop_ip));
					else
						printf(" %s", inet_ntoa(hop_ip));
					last_ip = hop_ip;
					ip_shown = 1;
				}
				printf("  %.3f ms", rtt);
				first = 0;
			}
			else
			{
				if (first)
					printf("  *");
				else
					printf(" *");
				first = 0;
			}
		}
		printf("\n");
		if (trt->reached)
			break;
		trt->ttl++;

	
	}

}







int	receiving_probe(t_trt *trt, struct	timeval *start, struct in_addr *hop_ip, double *rtt)
{

	char				buffer[512];
	struct sockaddr_in	from;
	socklen_t			fromlen = sizeof(from);
	struct timeval		end;
	ssize_t				n;
	int				 ip_hl;
	int				 icmp_type;
	int				 icmp_code;


	n = recvfrom(trt->recv_fd, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&from, &fromlen);
	gettimeofday(&end, NULL);

	
	if (n > 0)
	{
		ip_hl = (buffer[0] & 0x0F) * 4;

		if (n >= ip_hl + 2)
		{
			*rtt = (end.tv_sec - start->tv_sec) * 1000.0
				+ (end.tv_usec - start->tv_usec) / 1000.0;

			*hop_ip = from.sin_addr;

			icmp_type = buffer[ip_hl];
			icmp_code = buffer[ip_hl + 1];
			
			if (icmp_type == 3 && icmp_code == 3)
				trt->reached = true;
			return (1);
		}
		
		// for (int i = 0; i < n; i++)
		// {
		// 	printf("%02x ", (unsigned char)buffer[i]);
		// 	if ((i + 1) % 16 == 0)
		// 		printf("\n");
		// }
		// printf("\n");
	}

	else if (n < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (0);
		else
		{
			dprintf(2, "ft_traceroute: recvfrom: %s\n", strerror(errno));
			exit(1);
		}
	}
	return (0);
}
