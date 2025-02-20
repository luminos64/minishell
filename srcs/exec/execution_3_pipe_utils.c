/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_3_pipe_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:14:19 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/19 00:55:13 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_pipe(int *pipe, t_tokens *list)
{
	t_tokens	*temp;
	int			i;

	i = 1;
	temp = list;
	if (temp->status == PIPE)
		return (EXIT_FAILURE);
	while (temp)
	{
		if (temp->status == PIPE)
		{
			if (!temp->next || temp->next->status == PIPE)
				return (EXIT_FAILURE);
			i++;
		}
		temp = temp->next;
	}
	*pipe = i;
	return (EXIT_SUCCESS);
}

void	ft_put_error(char *str, char *cmd, int exit_code, int ctl)
{
	int	i;

	i = 0;
	if (ctl != WO_TITLE)
		write(STDERR_FILENO, "minishell: ", 11);
	while (cmd && cmd[i])
	{
		write(STDERR_FILENO, &cmd[i], 1);
		i++;
	}
	i = 0;
	while (str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	if (ctl != WO_NEW_LINE)
		write(STDERR_FILENO, "\n", 1);
	if (exit_code == N_PERMIT)
		free(cmd);
	if (exit_code)
		exit (exit_code);
}

void	ft_put_fd_error(char *str, char *name, int *fd)
{
	int	i;

	i = 0;
	close(fd[PIPE_WRITE]);
	write(STDERR_FILENO, "minishell: ", 11);
	while (name[i])
	{
		write(STDERR_FILENO, &name[i], 1);
		i++;
	}
	i = 0;
	while (str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	exit (EXIT_FAILURE);
}

static void	heredoc_2_get_line(char *eof, char *name)
{
	char	*line;
	int		fd;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
	if (fd < 0)
		exit (EXIT_FAILURE);
	while (1)
	{
		line = readline("\033[0;32m>> \033[0m");
		if (!line)
		{
			ft_put_error(eof, WARNING_HD, NOT_EXIT, WO_NEW_LINE);
			ft_putendl_fd("')", STDERR_FILENO);
			break ;
		}
		if (!ft_strncmp(line, eof, -1))
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
}

void	heredoc(t_isredirect *list)
{
	t_isredirect	*temp;
	char			*no;
	int				index;
	int				i;

	i = 0;
	index = 0;
	temp = list;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			if (index != temp->index)
				i = 0;
			i++;
			no = ft_strjoin3_agr(".temp", ft_itoa(temp->index), ft_itoa(i));
			heredoc_2_get_line(temp->name, no);
			free(no);
			index = temp->index;
		}
		temp = temp->next;
	}
}
