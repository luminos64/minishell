/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:29:01 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/20 20:54:47 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exce(char **cmd, t_minishell *id)
{
	char	**envp;
	char	*path;

	ft_builtins_child(id, cmd);
	path = get_path(id->env, cmd);
	envp = trans_env(id->env);
	if (!envp)
		ft_put_error(WARNING_MEM, "env: ", EXIT_FAILURE, W_TITLE);
	if (execve(path, cmd, envp) == -1)
	{
		ft_free_split(envp);
		if (errno == EACCES)
			ft_put_error(NO_PER, path, N_PERMIT, W_TITLE);
		free(path);
		if (errno == ENOENT)
			ft_put_error(": command not found", *cmd, N_FOUND, WO_TITLE);
		else
			ft_put_error(": execve failed:", *cmd, EXIT_FAILURE, W_TITLE);
	}
}

static void	ft_fork(t_minishell *id, t_ispipe *pipex, int index)
{
	char	**cmd;
	int		error_no;
	int		fd[2];
	int		pid;

	if (index > id->pipe)
		return ;
	cmd = find_cmd(id->cmd, index);
	pipe(fd);
	pid = fork();
	if (pid == -1)
	{
		ft_put_error(WARNING_MEM, "fork: ", EXIT_FAILURE, W_TITLE);
		return ;
	}
	if (pid == 0)
	{
		child_in(id, pipex, fd, index);
		exce(cmd, id);
	}
	close(fd[PIPE_WRITE]);
	dup2(fd[PIPE_READ], STDIN_FILENO);
	close(fd[PIPE_READ]);
	ft_fork(id, pipex, index + 1);
	waitpid(pid, &error_no, 0);
	// if (!ft_strncmp(cmd[0], "sleep", 6))
	// 	wait(&error_no);
	// else
	// 	waitpid(pid, &error_no, 0);
	if (index == id->pipe && error_no > 0)
		exit (error_no / 256);
}

static void	clear_heredoc_file(t_isredirect *fd)
{
	t_isredirect	*temp;
	char			*name;
	int				pipe;
	int				i;

	temp = fd;
	pipe = temp->index;
	i = 1;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			name = ft_strjoin3_agr(".temp", ft_itoa(temp->index), ft_itoa(i));
			unlink(name);
			free(name);
			i++;
		}
		temp = temp->next;
		if (temp && pipe != temp->index)
		{
			i = 1;
			pipe = temp->index;
		}
	}
}

static int	ft_pipe(t_minishell *id, t_fds *fd)
{
	t_ispipe		pipex;
	int				index;
	int				error_no;

	pipex.fds = fd;
	index = 1;
	if (fork() == 0)
	{
		signal(SIGINT, sig_handle_heredoc);
		heredoc(fd->in);
		exit (EXIT_SUCCESS);
	}
	wait(&error_no);
	if ((error_no) > 0)
	{
		clear_heredoc_file(fd->in);
		id->exit_status = error_no / 256;
		if (id->exit_status == EXIT_SIGINT)
			return (EXIT_SIGINT);
		return (EXIT_FAILURE);
	}
	signal(SIGINT, sig_handle_exec);
	signal(SIGQUIT, sig_handle_exec);
	ft_fork(id, &pipex, index);
	return (EXIT_SUCCESS);
}

void	execution(t_minishell *id, t_tokens *tokens)
{
	t_fds		*fd;
	int			error_no;

	ft_print_list(tokens, "tokens" ,1);
	error_no = 0;
	if (ft_getcmd(&id->cmd, tokens))
		error_no = EXIT_FAILURE;
	if (ft_redirect(&fd, tokens))
	{
		ft_put_error(WARNING_MEM, "fd: ", NOT_EXIT, W_TITLE);
		error_no = EXIT_FAILURE;
	}
	if (ft_count_pipe(&id->pipe, tokens))
	{
		ft_put_error(WARNING_PIPE, NULL, NOT_EXIT, W_TITLE);
		error_no = EXIT_FAILURE;
	}
	if (!error_no)
	{
		ft_print_cmd(id->cmd, "cmd");
		ft_print_list(fd->in, "in", 1);
		ft_print_list(fd->out, "out", 1);
		if (ft_builtins_main(id, tokens, fd))
		{
			if (fork() == 0)
				exit (ft_pipe(id, fd));
			wait(&id->exit_status);
			id->exit_status = id->exit_status / 256;
		}
	}
	else
		id->exit_status = error_no;
	ft_free_exec(tokens, id, fd);
}
