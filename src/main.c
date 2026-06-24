#include "../header/ft_traceroute.h"


int	main(int argc, char **argv)
{

	t_trt trt = {0};

	init(&trt);

	parse_args(argc, argv, &trt);

	init_hostname(&trt);

	init_socket(&trt);

	print_header(&trt);

	// printf("%s\n", trt.hostname);
	return (0);
}
