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
INCL		:= -I ${HDR_DIR}/ -I ${LIBFT_DIR}/include/

# Includes
HDR_FILES :=	minishell.h

# Files
SRC_FILES		:= lexer.c \
					parser.c \


SRC				:= ${addprefix ${SRC_DIR}/, ${SRC_FILES}}
OBJ				:= ${addprefix ${OBJ_DIR}/, ${SRC_FILES:.c=.o}}
HDR				:= ${addprefix ${HDR_DIR}/, ${HDR_FILES}}

# Colours
GREEN			:= \033[32;1m
YELLOW			:= \033[33;1m
RED				:= \033[31;1m
BOLD			:= \033[1m
RESET			:= \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@ printf "%b%s%b" "$(YELLOW)$(BOLD)" "Compiling MINISHELL..." "$(RESET)"
	@ $(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@ printf "\t\t%b%s%b\n" "$(GREEN)$(BOLD)" "	[OK]" "$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@ $(CC) $(CFLAGS) $(INCL) -c $< -o $@

$(OBJ_DIR):
	@ mkdir -p $(OBJ_DIR)

$(LIBFT):
	@ printf "%b%s%b" "$(YELLOW)$(BOLD)" "Compiling and archiving LIBFT..." "$(RESET)"
	@ make -C ${LIBFT_DIR}											> /dev/null
	@ printf "\t%b%s%b\n" "$(GREEN)$(BOLD)" "[OK]" "$(RESET)"

clean:
	@ echo "$(RED)$(BOLD) Cleaning LIBFT...$(RESET)"
	@ make clean -C ${LIBFT_DIR}								> /dev/null

	@ echo "$(RED)$(BOLD) Cleaning Object files...$(RESET)"
	@ rm -rf ${OBJ}
	@ rm -rf ${OBJ_DIR}

fclean:
	@ echo "$(RED)$(BOLD)Fully cleaning LIBFT...$(RESET)"
	@ make fclean -C ${LIBFT_DIR}								> /dev/null

	@ echo "$(RED)$(BOLD)Fully cleaning MINISHELL...$(RESET)"
	@ rm -rf ${NAME}
	@ rm -rf ${OBJ}
	@ rm -rf ${OBJ_DIR}

re: fclean $(NAME)

.PHONY: all clean fclean re