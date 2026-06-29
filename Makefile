NAME			= ft_traceroute
NAME_BONUS		= ft_traceroute_bonus
CC				= cc
CFLAGS			= -Wall -Werror -Wextra -g

SRC_DIR			= src
BONUS_DIR		= src_bonus
OBJ_DIR			= obj
OBJ_BONUS_DIR	= obj_bonus
HDR_DIR			= header

SRCS		= main.c init.c parse.c print.c utils.c probe.c
SRCS_BONUS	= main_bonus.c init_bonus.c parse_bonus.c print_bonus.c utils_bonus.c probe_bonus.c

OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS_BONUS	= $(addprefix $(OBJ_BONUS_DIR)/, $(SRCS_BONUS:.c=.o))

HEADERS			= $(HDR_DIR)/ft_traceroute.h
HEADERS_BONUS	= $(HDR_DIR)/ft_traceroute_bonus.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)

$(OBJ_BONUS_DIR)/%.o: $(BONUS_DIR)/%.c $(HEADERS_BONUS)
	@mkdir -p $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re