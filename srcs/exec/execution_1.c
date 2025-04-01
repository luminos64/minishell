/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:29:01 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/10 21:51:58 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exce(t_iscmd *cmd_l, t_minishell *id)
{
	char	**envp;
	char	**cmd;
	char	*path;

	if (cmd_l)
	{
		cmd = cmd_l->cmd;
		ft_builtins_child(id, cmd);
		path = get_path(id->env, cmd[0]);
		envp = trans_env(id->env);
		if (!envp)
			ft_put_error(WARNING_MEM, "env: ", EXIT_FAILURE, W_TITLE);
		if (execve(path, cmd, envp) == -1)
		{
			ft_free_split(envp);
			ft_put_error(": command not found", *cmd, N_FOUND, WO_TITLE);
		}
	}
	else
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (0);
	}
}

static void	ft_parent(t_minishell *id, t_iscmd *cmd)
{
	t_iscmd	*temp;

	if (cmd)
		wait(&cmd->status);
	else
		wait(NULL);
	temp = id->cmd;
	while (temp)
	{
		waitpid(temp->pid, &temp->status, 0);
		if (!temp->next)
			break ;
		temp = temp->next;
	}
	if (cmd)
		id->exit_status = temp->status / 256;
	if (id->exit_status)
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (id->exit_status);
	}
}

static void	ft_fork(t_minishell *id, t_redirect *fds, int index)
{
	t_iscmd	*cmd;
	int		fd[2];
	int		pid;

	if (index > id->pipe)
		return ;
	cmd = find_cmd(id->cmd, index);
	pipe(fd);
	pid = fork();
	if (cmd)
		cmd->pid = pid;
	if (pid == -1)
		ft_put_error(WARNING_MEM, "fork: ", EXIT_FAILURE, W_TITLE);
	if (pid == 0)
	{
		ft_child_clear(id, fds, EXIT_FAILURE);
		child_in(id, fds, fd, index);
		exce(cmd, id);
	}
	close(fd[PIPE_WRITE]);
	dup2(fd[PIPE_READ], STDIN_FILENO);
	close(fd[PIPE_READ]);
	ft_fork(id, fds, index + 1);
	ft_parent(id, cmd);
}

static int	ft_pipe(t_minishell *id, t_redirect *fd)
{
	int			index;
	int			error_no;

	index = 1;
	if (fork() == 0)
	{
		ft_child_clear(id, fd, EXIT_FAILURE);
		signal(SIGINT, sig_handle_heredoc);
		heredoc(fd, id);
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (EXIT_SUCCESS);
	}
	signal(SIGINT, SIG_IGN);
	wait(&error_no);
	if ((error_no) > 0)
		return (heredoc_error(id, fd, error_no));
	ft_child_clear(id, fd, EXIT_FAILURE);
	signal(SIGINT, sig_handle_exec);
	signal(SIGQUIT, sig_handle_exec);
	ft_fork(id, fd, index);
	ft_child_clear(NULL, NULL, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}

void	execution(t_minishell *id, t_tokens *tokens)
{
	t_redirect	*fd;
	int			error_no;

	fd = NULL;
	error_no = trans_tokens(id, &fd, tokens);
	// ft_print_list(tokens, "tokens", 0);
	ft_free_list(tokens);
	// ft_print_cmd(id->cmd, "cmd");
	// ft_print_list(fd, "fd", 0);
	if (!error_no)
	{
		if (ft_builtins_main(id, fd))
		{
			if (fork() == 0)
				exit (ft_pipe(id, fd));
			signal(SIGINT, SIG_IGN);
			wait(&error_no);
			id->exit_status = error_no / 256;
		}
	}
	else
		id->exit_status = error_no;
	ft_free_exec(id, fd);
	signal(SIGINT, sig_handle_line);
}
