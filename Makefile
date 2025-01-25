NAME = ft_ping
CC = gcc
CFLAGS = -Wall -Wextra -Werror

GREEN = \033[1;32m
RESET = \033[0m

SRCS = ft_ping.c ft_memcpy.c ft_memset.c utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Compilation completed successfully!$(RESET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
