NAME	= minishell

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -g

OBJ_DIR	= objs

SRC_DIR	= srcs/

PARSER	= extract.c parser1.c parser_utils.c token_list.c define_status.c

BUILDIN	= builtins.c get_env.c echo.c export.c export_value.c export_value2.c \
		cd.c exit.c

EXEC	= redirect.c get_cmd.c get_path.c execution_1.c \
		execution_2_child_in.c execution_3_pipe_utils.c execution_4_utils.c \
		trans_env.c

UTILS	= utils_1_list.c utils_2.c signal_handle.c

SOURCE	= main.c \
		$(addprefix exec/, $(EXEC)) \
		$(addprefix parser/, $(PARSER)) \
		$(addprefix builtins/, $(BUILDIN)) \
		$(addprefix utils/, $(UTILS))

SRCS	= $(addprefix $(SRC_DIR), $(SOURCE))

LIB_DIR	= libft

LIBFT	= ${LIB_DIR}/libft.a

INC_DIR	= includes

HEADER	= -I${LIB_DIR} \
		-I${INC_DIR}

OBJS	= ${SRCS:%.c=${OBJ_DIR}/%.o}

all:	${NAME}

${LIBFT}:
		${MAKE} -C ${LIB_DIR}

${NAME}: ${LIBFT} ${OBJS}
		${CC} -lreadline ${OBJS} ${CFLAGS} ${LIBFT} ${HEADER} -o ${NAME}
		@echo "\033[0;36mmini shell Compiled\n\033[0m"

${OBJ_DIR}/%.o:	%.c Makefile
		@mkdir -p ${dir $@}
		${CC} ${CFLAGS} ${HEADER} -c $< -o $@

# libs:
# 	@make -C ${LIB_DIR}

clean:
		rm -rfv ${OBJ_DIR}
		${MAKE} -C ${LIB_DIR} clean

fclean:	clean
		rm -rfv ${NAME}
		${MAKE} -C ${LIB_DIR} fclean
		@echo "\033[31mminishell Completed\n\033[0m"

re:		fclean all

.PHONY:	all clean fclean re
