NAME		:= minishell

# Directories
HDR_DIR		:= include
SRC_DIR		:= src
OBJ_DIR		:= obj

# Libft
LIBFT_DIR	:= libft
LIBFT		:= ${LIBFT_DIR}/libft.a

# Compiler flags
CC			:= cc
CFLAGS		:= -Wall -Werror -Wextra
INCL		:= -I ${HDR_DIR} -I ${LIBFT_DIR}/include/

# Readline detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    READLINE_LIB := -lreadline
endif
ifeq ($(UNAME_S),Darwin)
    READLINE_LIB := -L/opt/homebrew/opt/readline -lreadline
    INCL += -I/opt/homebrew/opt/readline/include
endif

# Includes
HDR_FILES :=	minishell.h

# Files
SRC_FILES	:= lexer/lexer_split.c \
				lexer/lexer_split2.c \
				lexer/lexer_list.c \
				parser/parser.c \
				parser/syntax.c \
				parser/redirections.c \
				parser/append.c \
				main.c \
				freeing.c \
				errors.c \
				init.c \
				executor/execute.c \
				executor/checks.c \
				executor/execute_builtin.c \
				executor/handle_redirects.c \
				executor/execute_child.c \
				builtins/export.c \
				expander/expander.c \
				expander/quotes.c \
				expander/replace.c \
				builtins/echo.c \
				builtins/unset.c \
				builtins/cd.c \
				builtins/pwd.c \
				builtins/env.c \
				builtins/exit.c \
				utils/env_utils.c \
				prompt.c \
				signals.c \
				heredoc/check.c \
				heredoc/handle.c


SRC			:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			:= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
HDR			:= $(addprefix $(HDR_DIR)/, $(HDR_FILES))

# Colours
GREEN		:= \033[32;1m
YELLOW		:= \033[33;1m
RED			:= \033[31;1m
BOLD		:= \033[1m
RESET		:= \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@ printf "%b%s%b" "$(YELLOW)$(BOLD)" "Compiling MINISHELL..." "$(RESET)"
	@ $(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(READLINE_LIB) -o $(NAME)
	@ printf "\t\t%b%s%b\n" "$(GREEN)$(BOLD)" "	[OK]" "$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@ mkdir -p $(@D)
	@ $(CC) $(CFLAGS) $(INCL) -c $< -o $@

$(OBJ_DIR):
	@ mkdir -p $(OBJ_DIR)

$(LIBFT):
	@ printf "%b%s%b" "$(YELLOW)$(BOLD)" "Compiling and archiving LIBFT..." "$(RESET)"
	@ make -C $(LIBFT_DIR)											> /dev/null
	@ printf "\t%b%s%b\n" "$(GREEN)$(BOLD)" "[OK]" "$(RESET)"

clean:
	@ echo "$(RED)$(BOLD) Cleaning LIBFT...$(RESET)"
	@ make clean -C $(LIBFT_DIR)								> /dev/null
	@ echo "$(RED)$(BOLD) Cleaning Object files...$(RESET)"
	@ rm -rf $(OBJ)
	@ rm -rf $(OBJ_DIR)

fclean:
	@ echo "$(RED)$(BOLD)Fully cleaning LIBFT...$(RESET)"
	@ make fclean -C $(LIBFT_DIR)								> /dev/null
	@ echo "$(RED)$(BOLD)Fully cleaning MINISHELL...$(RESET)"
	@ rm -rf $(NAME)
	@ rm -rf $(OBJ)
	@ rm -rf $(OBJ_DIR)

re: fclean $(NAME)

.PHONY: all clean fclean re
