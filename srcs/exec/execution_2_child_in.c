/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2_child_in.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 01:57:29 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/17 22:18:50 by usoontra         ###   ########.fr       */
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

static void	check_fd_in(t_isredirect *in, int limiter, int *fd)
{
	t_isredirect	*temp;
	char			*name;
	int				fd_temp;
	int				i;

	temp = in;
	i = 1;
	while (temp && temp->index == limiter)
	{
		if (temp->type == HERE_DOC)
		{
			name = ft_strjoin3_agr(".temp", ft_itoa(temp->index), ft_itoa(i));
			fd_temp = open(name, O_RDONLY, ALL_PERMIT);
		}
		else
		{
			if (access(temp->name, F_OK) < 0)
				ft_put_fd_error(NO_FILE, temp->name, fd);
			else if (access(temp->name, R_OK) < 0)
				ft_put_fd_error(NO_PER, temp->name, fd);
			fd_temp = open(temp->name, O_RDONLY, ALL_PERMIT);
		}
		check_fd_in_2_dup(fd_temp, temp->type, name, &i);
		temp = temp->next;
	}
}

static void	check_fd_out(t_isredirect *in, int limiter, int *fd)
{
	t_isredirect	*temp;
	int				fd_temp;

	temp = in;
	while (temp && temp->index == limiter)
	{
		if (temp->type == OUT_APPEND)
			fd_temp = open(temp->name, O_WRONLY | O_CREAT | O_APPEND, X_PERMIT);
		else
			fd_temp = open(temp->name, O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
		if (access(temp->name, W_OK) < 0)
			ft_put_fd_error(NO_PER, temp->name, fd);
		dup2(fd_temp, STDOUT_FILENO);
		close(fd_temp);
		temp = temp->next;
	}
}

static t_isredirect	*find_fd(t_isredirect *list, int index)
{
	t_isredirect	*temp;

	temp = list;
	while (temp)
	{
		if (temp->index == index)
			break ;
		temp = temp->next;
	}
	return (temp);
}

void	child_in(t_minishell *id, t_ispipe *pipex, int *fd, int index)
{
	t_isredirect	*temp_in;
	t_isredirect	*temp_out;

	close(fd[PIPE_READ]);
	temp_in = find_fd(pipex->fds->in, index);
	temp_out = find_fd(pipex->fds->out, index);
	if (temp_in)
		check_fd_in(temp_in, index, fd);
	if (temp_out)
		check_fd_out(temp_out, index, fd);
	else if (index == id->pipe && !temp_out)
		dup2(STDOUT_FILENO, STDOUT_FILENO);
	else
		dup2(fd[PIPE_WRITE], STDOUT_FILENO);
	close(fd[PIPE_WRITE]);
}
