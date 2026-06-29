#include "../header/ft_traceroute_bonus.h"

bool	is_number(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int parse_number(char *optarg, int opt)
{
    if (!is_number(optarg))
    {
        dprintf(2, "Cannot handle `-%c' option with arg `%s' (argc %d)\n",
            opt, optarg, optind - 1);
        exit(2);
    }
    return ((int)strtol(optarg, NULL, 10));
}

void	parse_args(int argc, char **argv, t_trt *trt)
{
	int opt;
	int option_index = 0;
	static struct option long_options[] = {
		{"max-hops",	required_argument,	0, 'm'},
		{"port",		required_argument,	0, 'p'},
		{"queries",		required_argument,	0, 'q'},
		{"first",		required_argument,	0, 'f'},
		{"help",		no_argument,		0, 'h'},
		{0, 0, 0, 0}
	};

	opterr = 0;
	while ((opt = getopt_long(argc, argv, ":m:p:q:f:n", long_options, &option_index)) != -1)
	{
		switch (opt) {
			case 'm':
				trt->max_hops = parse_number(optarg, opt);
				if (trt->max_hops < 1 || trt->max_hops > 255)
				{
					dprintf(2, "max hops cannot be more than 255\n");
					exit(2);
				}
				break;
            case 'p':
				trt->initial_port = parse_number(optarg, opt);
				break;
            case 'q':
				trt->nprobes = parse_number(optarg, opt);
				if (trt->nprobes < 1 || trt->nprobes > 10)
				{
					dprintf(2, "no more than 10 probes per hop\n");
					exit(2);
				}
				break;
            case 'f':
				trt->first_ttl = parse_number(optarg, opt);
				break;
            case 'n':
				trt->numeric      = true;
				break;
            case 'h': print_help(1); break;
            case ':':
                dprintf(2, "ft_traceroute: option requires an argument -- '%c'\n", optopt);
                exit(2);
            case '?':
                dprintf(2, "ft_traceroute: invalid option -- '%c'\n", optopt);
                exit(2);
		}
	}
	if (trt->first_ttl < 1 || trt->first_ttl > trt->max_hops)
	{
		dprintf(2, "first hop out of range\n");
		exit(2);
	}
	if (argv[optind])
	{
		ft_strlcpy(trt->hostname, argv[optind], HOSTNAME_SIZE);
	}
	else
	{
		fprintf(stderr,"ft_traceroute: missing host operand\n");
		exit(2);
	}
}
