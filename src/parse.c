#include "../header/ft_traceroute.h"

void	parse_args(int argc, char **argv, t_trt *trt)
{
	int i;

	i = 1;

	if (argc < 2)
		print_help(2);
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "--help") == 0)
			print_help(1);

		else if (argv[i][0] == '-')
		{
			dprintf(2, "Bad option `%s' (argc %d)\n", argv[i], i);
			exit(2);
		}
		else
			ft_strlcpy(trt->hostname, argv[i], HOSTNAME_SIZE);
		i++;
	}
}
