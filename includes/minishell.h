/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:29:45 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/10 21:13:54 by usoontra         ###   ########.fr       */
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
# define NOT_DIR ": Not a directory"

// status -1 = not assgined | 0 = cmd
// | 1 = cmd element
// | 2 = pipe "|"
// | 3 = fd
// | 4 = file_name
// | 5 = "="
typedef struct s_tokens
{
	struct s_tokens	*next;
	char			*str;
	int				status;
}	t_tokens;

typedef struct s_iscmd
{
	struct s_iscmd	*next;
	// t_tokens		*equal;
	char			**cmd;
	int				status;
	int				index;
	int				pid;
}	t_iscmd;

// type 1 = in | 2 = out | 3 = here doc
// | 4 = out appen
typedef struct s_redirect
{
	struct s_redirect	*next;
	char				*name;
	int					type;
	int					index; // No. pipe
}	t_redirect;

//***** when dont have "=" */
// status -1 not echo 0 = echo | 1 = export | 2 = export + env | 3 = env
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
	int			pipe;
	t_env		*env;
	t_env		*home;
	t_env		*user;
	t_env		*pwd;
	t_env		*old_pwd;
	t_env		*shellvl;
	t_iscmd		*cmd;
	long long	exit_status;
}	t_minishell;

// main

// exec/exe 1
void		execution(t_minishell *id, t_tokens *tokens);

// exec/exe 2
int			heredoc_error(t_minishell *id, t_redirect *fd, int error_no);
void		heredoc(t_redirect *list, t_minishell *id);

// exec/exe 3
void		child_in(t_minishell *id, t_redirect *fds, int *fd, int index);

// exec/exe 4
char		*get_path(t_env *env, char *cmd);

// exec/exe 5
char		**trans_env(t_env *list);

// exec/get_cmd
int			ft_getcmd(t_iscmd **cmd, t_tokens *list);

// exec/get_env
int			ft_getenv(t_minishell *id, t_env **env, char **envp);
void		ft_print_env(t_env *env, char **cmd);

// exec/redirect
int			ft_redirect(t_redirect **fd, t_tokens *list);

// utils/signal_handle
void		check_signal_line(long long *ctl);
void		check_signal_heredoc(char *name);
void		sig_handle_heredoc(int sig_no);
void		sig_handle_exec(int sig_no);
void		sig_handle_line(int sig_no);

// utils/utils_1_list
int			ft_strlen_char(char *str, char c);
void		ft_list_addback(void **list, void *new);
int			ft_list_count(void *list);
void		ft_print_list(void *list, char *str, int ctl);
void		ft_print_cmd(t_iscmd *cmd, char *str);

// exec/utils 2
int			ft_count_pipe(int *pipe, t_tokens *list);
t_iscmd		*find_cmd(t_iscmd *list, int limit);
char		*ft_strjoin3_a(char *s1, char *s2, char *s3);
int			trans_tokens(t_minishell *id, t_redirect **fd, t_tokens *tokens);

// exec/utils_3
char		*check_path(char *cmd);
void		ft_put_error(char *str, char *cmd, int exit_code, int ctl);
void		ft_put_fd_error(char *str, char *name, int *fd);

// exec/utils_4
void		ft_free_list(void *list);
void		ft_free_split(char **cmd);
void		ft_free_exec(t_minishell *id, t_redirect *fd);
void		ft_free_env(t_env *env);
void		ft_child_clear(t_minishell *id, t_redirect *fd, int ctl);

// builtins/builtins
int			ft_builtins_main(t_minishell *id, t_redirect *fd);
void		ft_builtins_child(t_minishell *id, char **cmd);
int			check_correct_name(char *name, int *len);

// builtins/cd
int			cd_main(t_minishell *id, char	**cmd);
void		cd_child(t_minishell *id, char **cmd);

// builtins/exit
void		ft_exit_child(t_minishell *id, char **cmd);
void		ft_exit_main(t_minishell *id);

// builtins/export_value
void		add_export_value(t_minishell *id, t_env **env, char **cmd);

// builtins/export
void		ft_print_export(t_env *env);

// builtins/unset
void		ft_unset(t_minishell *id, t_env **env, char **name);

// =========================================================================
// parser

// parser/parser1
int			is_operator(char c, int ctl);
t_tokens	*check_parser(char *input, t_minishell *id);

// parser/extract
char		*ft_strncpy(const char *src, int len);
char		*get_env_value(t_minishell *id, const char *var_name);
char		*extrack_token(t_minishell *id, char *str, int *len, int start);

// parser/extract_quote
char		*ft_qoute_screen(t_minishell *id, char *str, int *i, char *head);

// parser/expand_line
char		*ft_expand_line(char *str, t_minishell *id, char *res);

// parser/token_list
int			screen_space(char c);
void		add_node(t_tokens **list, char *token, int *ctl, int equal);

#endif
