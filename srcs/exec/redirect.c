/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:28:56 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/10 19:19:07 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error_fd(t_tokens *list)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	if (list)
	{
		ft_putstr_fd(list->str, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
}

static int	add_fd(t_redirect **fd, int pipe_no, t_tokens *list, int type)
{
	t_redirect	*temp;

	temp = malloc(sizeof(t_redirect));
	if (!temp)
		return (EXIT_FAILURE);
	temp->name = ft_strdup(list->str);
	temp->type = type;
	temp->index = pipe_no;
	temp->next = NULL;
	ft_list_addback((void **)fd, temp);
	return (EXIT_SUCCESS);
}

static int	ft_redirect_2(t_redirect **fd, int pipe_no, t_tokens *list)
{
	int				type;

	type = 0;
	if (!ft_strncmp(list->str, ">>", 3))
		type = OUT_APPEND;
	else if (!ft_strncmp(list->str, ">", 1))
		type = OUT_TRUNC;
	else if (!ft_strncmp(list->str, "<<", 3))
		type = HERE_DOC;
	else if (!ft_strncmp(list->str, "<", 2))
		type = FD_IN;
	list = list->next;
	if (!list || list->status != REDIRECT_NAME)
	{
		print_error_fd(list);
		return (EXIT_FAILURE);
	}
	if (add_fd(fd, pipe_no, list, type))
	{
		ft_put_error(WARNING_MEM, "fd: ", NOT_EXIT, W_TITLE);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_redirect(t_redirect **fd, t_tokens *list)
{
	t_tokens	*temp;
	int			pipe_no;

	temp = list;
	pipe_no = 1;
	while (temp)
	{
		if (temp->status == PIPE)
			pipe_no++;
		if (temp->status == REDIRECT)
		{
			if (ft_redirect_2(fd, pipe_no, temp))
				return (EXIT_FAILURE);
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}
