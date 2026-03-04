INCLUDES=includes/
SRCS=pipex.c pipex_tools.c
SRCS_BONUS=pipex_bonus.c pipex_tools_bonus.c exec_bonus.c
OBJDIR=objs/
OBJECTS=$(addprefix $(OBJDIR), $(SRCS:.c=.o))
OBJECTS_BONUS=$(addprefix $(OBJDIR), $(SRCS_BONUS:.c=.o))

LIBFT_DIR=libs/libft_tools/
LIBFT=$(addprefix $(LIBFT_DIR), libft_tools.a)

CC=cc
VPATH=srcs
CFLAGS=-Werror -Wall -Wextra -g
NAME=pipex
BONUS=pipex_bonus

all: $(NAME)

$(NAME): $(OBJECTS_BONUS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDES)
	@echo "Finished compiling Pipex"

bonus: $(OBJECTS_BONUS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(BONUS) $^ -I$(INCLUDES)
	@echo "Finished compiling Pipex bonuses"

$(OBJDIR)%.o: %.c | $(OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBFT):
	@make -sC $(LIBFT_DIR) all
	@echo "Compiled library Libft"

clean:
	@rm -drf $(OBJDIR)
	@make -sC $(LIBFT_DIR) clean
	@echo "Cleaned Pipex object files"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(BONUS)
	@make -sC $(LIBFT_DIR) fclean
	@echo "Cleaned Pipex"

re: clean all
	@echo "Recompiled Pipex"

.PHONY: all clean fclean re bonus
