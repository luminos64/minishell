/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:14:19 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/03 19:31:18 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_heredoc_file(t_redirect *fd)
{
	t_redirect	*temp;
	char		*name;
	int			pipe;
	int			i;

	temp = fd;
	pipe = temp->index;
	i = 1;
	while (temp)
	{
		if (temp->type == HERE_DOC)
		{
			name = ft_strjoin3_a(".temp", ft_itoa(temp->index), ft_itoa(i));
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

int	heredoc_error(t_minishell *id, t_redirect *fd, int error_no)
{
	ft_child_clear(id, fd, EXIT_FAILURE);
	clear_heredoc_file(fd);
	ft_child_clear(NULL, NULL, EXIT_SUCCESS);
	error_no = error_no / 256;
	if (error_no == EXIT_SIGINT)
		return (EXIT_SIGINT);
	return (EXIT_FAILURE);
}

static int	ft_heredoc_expand(char *str, t_minishell *id, int fd, char *eof)
{
	char	*res;

	res = NULL;
	res = ft_expand_line(str, id, res);
	if (!res)
	{
		res = ft_strdup("\n");
	}
	if (!ft_strncmp(res, eof, -1))
	{
		free(res);
		return (EXIT_SUCCESS);
	}
	ft_putendl_fd(res, fd);
	free(res);
	return (EXIT_FAILURE);
}

static void	heredoc_2_get_line(char *eof, char *name, t_minishell *id)
{
	char	*line;
	int		fd;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
	if (fd < 0)
		exit (EXIT_FAILURE);
	check_signal_heredoc(name, fd);
	while (1)
	{
		line = readline("\033[0;32m>> \033[0m");
		if (!line)
		{
			ft_put_error(eof, WARNING_HD, NOT_EXIT, WO_NEW_LINE);
			ft_putendl_fd("')", STDERR_FILENO);
			break ;
		}
		if (!ft_heredoc_expand(line, id, fd, eof))
			break ;
	}
	close(fd);
}

void	heredoc(t_redirect *list, t_minishell *id)
{
	t_redirect	*temp;
	char		*no;
	int			index;
	int			i;

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
			no = ft_strjoin3_a(".temp", ft_itoa(temp->index), ft_itoa(i));
			heredoc_2_get_line(temp->name, no, id);
			free(no);
			index = temp->index;
		}
		temp = temp->next;
	}
}
