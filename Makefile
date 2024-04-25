NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

LIBFT = libft
LIBFT_LIB = $(LIBFT)/libft.a

SRC_FILES = lexer.c
OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

# Rule to make everything
all: $(OBJ_DIR) $(NAME)

# Creating the obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compiling the project and linking with libft to create the push_swap executable
$(NAME): $(LIBFT_LIB) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) -L$(LIBFT) -lft

# Rule for making libft
$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT)

# Generic rule for object files, now includes obj directory
$(OBJ_DIR)/%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning up the object files
clean:
	$(RM) -r $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT)

# Full clean (includes the library and executable)
fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C $(LIBFT)

# Re-make everything
re: fclean all

# Prevents issues with make and filenames
.PHONY: all clean fclean re