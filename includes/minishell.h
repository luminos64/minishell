/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:29:45 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/20 20:54:01 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>

# define RED "\033[1;91m"
# define PINK "\033[1;38;2;255;182;193m"
# define RESET "\033[0m"

# define NOT_ASSIGN -1
# define PRINT_ECHO 0
# define PRINT_EX 1
# define PRINT_EX_ENV 2
# define PRINT_ENV 3

# define CMD 0
# define CMD_ELEMENT 1
# define PIPE 2
# define REDIRECT 3
# define REDIRECT_NAME 4
# define EXPAND 5

# define FD_IN 1
# define OUT_TRUNC 2
# define HERE_DOC 3
# define OUT_APPEND 4

# define WO_TITLE 0
# define W_TITLE 1
# define WO_NEW_LINE 2

# define ALL_PERMIT 0777
# define X_PERMIT 0644

# define PIPE_READ 0
# define PIPE_WRITE 1

# define NOT_EXIT 0
# define INVALID 2
# define N_PERMIT 126
# define N_FOUND 127
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131

# define WARNING_MEM "memory allocated error"
# define WARNING_HD "warning: here-document delimited by end-of-file (wanted `"
# define WARNING_PIPE "syntax error near unexpected token `|'"
# define NO_FILE ": No such file or directory"
# define NO_PER ": Permission denied"

// status -1 = not assgined | 0 = cmd
// | 1 = cmd element
// | 2 = pipe "|"
// | 3 = fd
// | 4 = file_name
typedef struct s_tokens
{
	struct s_tokens	*next;
	char			*str;
	int				status;
}	t_tokens;

typedef struct s_iscmd
{
	struct s_iscmd	*next;
	char			**cmd;
	int				index;
}	t_iscmd;

// type 1 = in | 2 = out | 3 = here doc
// | 4 = out appen
typedef struct s_isredirect
{
	struct s_isredirect	*next;
	char				*name;
	int					type;
	int					index; // No. pipe
}	t_isredirect;

typedef struct s_fds
{
	t_isredirect	*in;
	t_isredirect	*out;
}	t_fds;

typedef struct s_ispipe
{
	t_fds	*fds;
}	t_ispipe;

//***** when dont have "=" */
// status 0 = echo | 1 = export | 2 = export + env | 3 = env
// print 0 = not print | 1 = already print
// export ต้องเรียงตัวอักษร A-Z > a-z > _ | start with CHROME_
typedef struct s_env
{
	struct s_env	*next;
	char			*name;
	char			*value;
	int				status;
	int				print_index;
}	t_env;

typedef struct s_minishell
{
	int		pipe;
	int		exit_status;
	t_env	*env;
	t_env	*shellvl;
	t_env	*pwd;
	t_env	*old_pwd;
	t_env	*user;
	t_iscmd	*cmd;
}	t_minishell;

// main
void	ft_free_env(t_env *env);

// exec/get_cmd
int		ft_getcmd(t_iscmd **cmd, t_tokens *list);

// exec/get_path
char	*get_path(t_env *env, char **cmd);

// exec/redirect
int		ft_redirect(t_fds **fd, t_tokens *list);

// exec/exe 1
void	execution(t_minishell *id, t_tokens *tokens);

// exec/exe 2
void	child_in(t_minishell *id, t_ispipe *pipex, int *fd, int index);

// exec/exe 3
int		ft_count_pipe(int *pipe, t_tokens *list);
void	ft_put_error(char *str, char *cmd, int exit_code, int ctl);
void	ft_put_fd_error(char *str, char *name, int *fd);
void	heredoc(t_isredirect *list);

// exec/exe 4
char	**find_cmd(t_iscmd *list, int limit);
char	*ft_strjoin3_agr(char *s1, char *s2, char *s3);
void	ft_print_cmd(t_iscmd *cmd, char *str);
void	ft_free_split(char **cmd);
void	ft_free_exec(t_tokens *list, t_minishell *id, t_fds *fd);

// exec/tranc_env
char	**trans_env(t_env *list);

// builtins/builtins
int		ft_builtins_main(t_minishell *id, t_tokens *tokens, t_fds *fd);
void	ft_builtins_child(t_minishell *id, char **cmd);

// builtins/export
void	ft_print_export(t_env *env);

// builtins/export_value
void	add_export_value(t_env **env, t_tokens *tokens);

// builtins/export_value2
// int		check_export_name(char *name);
int 	check_export_name(char *name, char *value);
int		parse_token(t_tokens *current, char **name, char **value);

// builtins/get_env
int		ft_getenv(t_minishell *id,t_env **env, char **envp);
void	ft_print_env(t_env *env);

// builtins/cd
int		cd_main(t_minishell *id, char	**cmd);
void	cd_child(char **cmd);

// buildins/echo
void	print_echo(char **cmd);

// builtins/exit
void	ft_exit_child(t_minishell *id, char **cmd);
void	ft_exit_main(t_minishell *id, t_tokens *tokens, t_fds *fd);

// utils/signal_handle
void	check_signal(int *ctl);
void	sig_handle_heredoc(int sig_no);
void	sig_handle_exec(int sig_no);
void	sig_handle_line(int sig_no);

// utils/utils_1_list
void	ft_list_addback(void **list, void *new);
int		ft_list_count(void *list);
void	ft_print_list(void *list, char *str, int ctl);
void	ft_free_list(void *list);

// utils/utils_2
int		ft_strlen_char(char *str, char c);

// =========================================================================
// parser

// parser/parser1
int			count_tokens(const char *input);
int			get_token_len(const char *input);
void		tokenize_input(char *input, t_tokens **head, t_minishell *id);
t_tokens	*check_parser(char *input, t_minishell *id);
// t_tokens	*check_parser(char *input, t_env *env);
// void		tokenize_input(char *input, t_tokens **head, t_env *env);

// parser/parser_utils
int			is_operator(char c);
char		*ft_strncpy(const char *src, int len);
void		ft_free(char **str);
char		*get_env_value(t_minishell *id, const char *var_name);
// char		*get_env_value(t_env *env, const char *var_name);

// parser/extract
char		*extract_quoted_string(const char *input, int *index);
char		*extract_operator(const char *input, int *index);
char		*extract_regular_token(const char *input, int *index);
char		*expand_variable(const char *input, int *index, t_minishell *id);
// char		*expand_variable(const char *input, int *index, t_env *env);

// parser/token_list
t_tokens	*create_node(const char *token);
void		add_node(t_tokens **list, char *token, int *ctl);
void		free_tokens_list(t_tokens *head);

// parser/define_status
int			define_status(char *str, int ctl);
int			check_status_isredirect(t_tokens **list);

// void	add_env_value(t_env *env, char *envp, int len_name);
void		print_export(t_minishell *id, char **cmd);

#endif

// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
