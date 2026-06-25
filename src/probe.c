#include "../header/ft_traceroute.h"

void	print_result(t_trt *trt)
{
	if (trt->replied)
	{
		if (!trt->ip_shown || trt->hop_ip.s_addr != trt->last_ip.s_addr)
		{
			if (trt->first)
				printf("  %s", inet_ntoa(trt->hop_ip));
			else
				printf(" %s", inet_ntoa(trt->hop_ip));
			trt->last_ip = trt->hop_ip;
			trt->ip_shown = 1;
		}
		printf("  %.3f ms", trt->rtt);
		trt->first = 0;
	}
	else
	{
		if (trt->first)
			printf("  *");
		else
			printf(" *");
		trt->first = 0;
	}
}


void	send_one(t_trt *trt)
{
	char	payload[32] = {0};

	gettimeofday(&trt->start, NULL);
	trt->dest_addr.sin_port = htons(trt->initial_port + trt->probes_sent);
	if (sendto(trt->send_fd, payload, sizeof(payload), 0,
		(struct sockaddr *)&trt->dest_addr, sizeof(trt->dest_addr)) < 0)
	{
		dprintf(2, "ft_traceroute: sendto: %s\n", strerror(errno));
		exit(1);
	}
	trt->expected_port = trt->initial_port + trt->probes_sent;
	trt->replied = receiving_probe(trt);
	trt->probes_sent++;
	print_result(trt);
}


void	sending_probe(t_trt *trt)
{

	trt->ttl = 1;
	trt->dest_addr.sin_port = htons(trt->initial_port);
	while (trt->ttl <= trt->max_hops)
	{
		if (setsockopt(trt->send_fd, IPPROTO_IP, IP_TTL, &trt->ttl, sizeof(trt->ttl)))
		{
			dprintf(2, "ft_traceroute: setsockopt: %s\n", strerror(errno));
			exit (1);
		}
		printf("%2d", trt->ttl);
		trt->ip_shown = 0;
		trt->first = 1;
		for (int i = 0; i < 3; i++)
			send_one(trt);
		printf("\n");
		if (trt->reached)
			break;
		trt->ttl++;
	}
}




int	receiving_probe(t_trt *trt)
{
	char				buffer[512];
	struct sockaddr_in	from;
	socklen_t			fromlen = sizeof(from);
	struct timeval		end;
	ssize_t				n;

	struct ip		*ip_ext; // Outer IP header (router's packet).
	struct icmp		*icmp;   // ICMP header (error message).
	int				 hl_ext; // Outer IP header length, in Bytes.

	while (1)
	{
		// Receive raw ICMP packet on raw socket.
		// from is filled with the replying router address.
		n = recvfrom(trt->recv_fd, buffer, sizeof(buffer), 0,
					(struct sockaddr *)&from, &fromlen);
		gettimeofday(&end, NULL);

		if (n > 0)
		{
			// Layer 1: starts at the beginning of the buffer, cast buffer to (struct ip *) to read its fields.
			ip_ext = (struct ip *)buffer;
			// ip_hl IP header length, in 4-byte owrds.
			hl_ext = ip_ext->ip_hl * 4;
			
			// Layer 2: ICMP header starts right after the outer IP.
			icmp = (struct icmp *)(buffer + hl_ext);
			trt->rtt = (end.tv_sec - trt->start.tv_sec) * 1000.0
					+ (end.tv_usec - trt->start.tv_usec) / 1000.0;
			// Router IP, source of the outer packet.
			trt->hop_ip = from.sin_addr;
			// type 3 (destination unreachable), code 3 (port unreachable).
			if (icmp->icmp_type == 3 && icmp->icmp_code == 3)
				trt->reached = true;

			// Layer 3: inner IP header (our original packet, copied by
			// the router) starts after the ICMP header (8 bytes).
			struct ip	*ip_in = (struct ip *)(buffer + hl_ext + 8);
			int			 hl_in = ip_in->ip_hl * 4;	// inner IP header length

			// Layer 4: inner UDP header (our copy) starts after the inner IP.
			struct udphdr	*udp_in = (struct udphdr *)((char *)ip_in + hl_in);

			// The destination port we originally sent, copied back by the
			// router. ntohs converts from network to host byte order.
			uint16_t	dport = ntohs(udp_in->uh_dport);

				
			return (1);
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
	}
	return (0);
}
