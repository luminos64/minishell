/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3_child_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 01:57:29 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 19:54:40 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_fd_in_2_dup(int fd, int type, char *name, int *i)
{
	if (fd < 0)
	{
		if (type == HERE_DOC)
		{
			unlink(name);
			free(name);
		}
		ft_put_error(WARNING_MEM, "fd: ", EXIT_FAILURE, W_TITLE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (type == HERE_DOC)
	{
		(*i)++;
		unlink(name);
		free(name);
	}
}

static void	check_fd_in(t_redirect *in, int limiter)
{
	t_redirect	*temp;
	char		*name;
	int			fd_temp;
	int			i;

	temp = in;
	i = 1;
	while (temp && temp->index == limiter)
	{
		if (temp->type == HERE_DOC || temp->type == FD_IN)
		{
			if (temp->type == HERE_DOC)
			{
				name = ft_strjoin3_a(".temp", ft_itoa(temp->index), ft_itoa(i));
				fd_temp = open(name, O_RDONLY, ALL_PERMIT);
			}
			else if (temp->type == FD_IN)
				fd_temp = open(temp->name, O_RDONLY, ALL_PERMIT);
			check_fd_in_2_dup(fd_temp, temp->type, name, &i);
		}
		temp = temp->next;
	}
}

static int	check_fd_out(t_redirect *in, int limiter)
{
	t_redirect	*temp;
	int			fd;
	int			ctl;

	temp = in;
	ctl = 0;
	while (temp && temp->index == limiter)
	{
		if (temp->type == OUT_APPEND || temp->type == OUT_TRUNC)
		{
			ctl++;
			if (temp->type == OUT_APPEND)
				fd = open(temp->name, O_WRONLY | O_CREAT | O_APPEND, X_PERMIT);
			else if (temp->type == OUT_TRUNC)
				fd = open(temp->name, O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		temp = temp->next;
	}
	return (ctl);
}

static void	ft_check_fd(t_redirect *fds, int limiter, int *fd)
{
	t_redirect	*temp;
	int			out;

	temp = fds;
	while (temp && temp->index == limiter)
	{
		if (temp->type == FD_IN)
		{
			if (access(temp->name, F_OK) < 0)
				ft_put_fd_error(NO_FILE, temp->name, fd);
			else if (access(temp->name, R_OK) < 0)
				ft_put_fd_error(NO_PER, temp->name, fd);
		}
		else if (temp->type == OUT_APPEND || temp->type == OUT_TRUNC)
		{
			if (temp->type == OUT_APPEND)
				out = open(temp->name, O_WRONLY | O_CREAT | O_APPEND, X_PERMIT);
			else
				out = open(temp->name, O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
			if (access(temp->name, W_OK) < 0)
				ft_put_fd_error(NO_PER, temp->name, fd);
			close(out);
		}
		temp = temp->next;
	}
}

void	child_in(t_minishell *id, t_redirect *fds, int *fd, int index)
{
	t_redirect	*temp;
	int			out;

	close(fd[PIPE_READ]);
	temp = fds;
	while (temp)
	{
		if (temp->index == index)
			break ;
		temp = temp->next;
	}
	ft_check_fd(temp, index, fd);
	if (temp)
		check_fd_in(temp, index);
	out = check_fd_out(temp, index);
	if (index == id->pipe && !out)
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	else if (!out)
		dup2(fd[PIPE_WRITE], STDOUT_FILENO);
	close(fd[PIPE_WRITE]);
}
