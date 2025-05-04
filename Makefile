NAME	= minishell

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -g

OBJ_DIR	= objs

SRC_DIR	= srcs

PARSER	= parser1.c extract.c extract_quote.c token_list.c expand_line.c

BUILDIN	= builtins.c export.c export_value.c cd.c exit.c unset.c

EXEC	= redirect.c get_cmd.c signal_handle.c get_env.c\
		execution_1.c execution_2_heredoc.c execution_3_child_in.c \
		execution_4_get_path.c execution_5_trans_env.c \
		utils_1_list.c utils_2.c utils_3_put_error.c utils_4_free.c

SOURCE	= main.c \
		$(addprefix exec/, $(EXEC)) \
		$(addprefix parser/, $(PARSER)) \
		$(addprefix builtins/, $(BUILDIN))

LIB_DIR	= libft

LIBFT	= ${LIB_DIR}/libft.a

INC_DIR	= includes

HEADER	= -I${LIB_DIR} -I${INC_DIR}

OBJS	= ${SOURCE:%.c=${OBJ_DIR}/%.o}

all:	${NAME}

${LIBFT}:
		${MAKE} -C ${LIB_DIR}

${NAME}: ${LIBFT} ${OBJS}
		${CC} ${OBJS} ${LIBFT} ${CFLAGS} ${HEADER} -o ${NAME} -lreadline -lhistory -lncurses
		@echo "\033[0;36mmini shell Compiled\n\033[0m"

$(OBJS):	${OBJ_DIR}/%.o:	$(SRC_DIR)/%.c Makefile ./includes/minishell.h
		@mkdir -p ${dir $@}
		${CC} ${CFLAGS} ${HEADER} -c $< -o $@

clean:
		rm -rfv ${OBJ_DIR}
		${MAKE} -C ${LIB_DIR} clean

fclean:	clean
		rm -rfv ${NAME}
		${MAKE} -C ${LIB_DIR} fclean
		@echo "\033[31mminishell Clean Completed\n\033[0m"

re:		fclean all

v:
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes ./minishell
t:
	valgrind --suppressions=readline.supp  ./minishell

.PHONY:	all clean fclean re
