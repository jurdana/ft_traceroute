NAME	= ft_traceroute
CC		= cc
CFLAGS	= -Wall -Werror -Wextra -g

SRC_DIR = src
OBJ_DIR	= obj
HDR_DIR = header

SRCS	= main.c init.c parse.c print.c utils.c probe.c
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
HEADERS = $(HDR_DIR)/ft_traceroute.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

test1: all
	@./ft_traceroute 8.8.8.8

test2: all
	@./ft_traceroute google.com

help: all
	@./ft_traceroute --help


.PHONY: all clean fclean re