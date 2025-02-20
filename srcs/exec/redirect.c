/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:28:56 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/17 22:21:38 by usoontra         ###   ########.fr       */
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
		ft_putstr_fd("newline", STDERR_FILENO);
}

static int	add_fd_out(t_isredirect **fd, int pipe_no, t_tokens *list)
{
	int				type;
	t_isredirect	*temp;

	type = 0;
	if (!ft_strncmp(list->str, "<", 1))
		return (EXIT_SUCCESS);
	if (!ft_strncmp(list->str, ">>", 3))
		type = OUT_APPEND;
	else if (!ft_strncmp(list->str, ">", 2))
		type = OUT_TRUNC;
	list = list->next;
	if (!list || list->status != REDIRECT_NAME)
	{
		print_error_fd(list);
		return (EXIT_FAILURE);
	}
	temp = malloc(sizeof(t_isredirect));
	if (!temp)
		return (EXIT_FAILURE);
	temp->name = list->str;
	temp->type = type;
	temp->index = pipe_no;
	temp->next = NULL;
	ft_list_addback((void **)fd, temp);
	return (EXIT_SUCCESS);
}

static int	add_fd_in(t_isredirect **fd, int pipe_no, t_tokens *list)
{
	int				type;
	t_isredirect	*temp;

	type = 0;
	if (!ft_strncmp(list->str, ">", 1))
		return (EXIT_SUCCESS);
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
	temp = malloc(sizeof(t_isredirect));
	if (!temp)
		return (EXIT_FAILURE);
	temp->name = list->str;
	temp->type = type;
	temp->index = pipe_no;
	temp->next = NULL;
	ft_list_addback((void **)fd, temp);
	return (EXIT_SUCCESS);
}

static int	ft_redirect_2(t_fds **fd, int pipe_no, t_tokens *list)
{
	int	error_status;

	error_status = EXIT_SUCCESS;
	if (add_fd_in(&(*fd)->in, pipe_no, list))
		error_status = EXIT_FAILURE;
	if (add_fd_out(&(*fd)->out, pipe_no, list))
		error_status = EXIT_FAILURE;
	if (error_status)
	{
		ft_free_list((*fd)->in);
		ft_free_list((*fd)->out);
		free(*fd);
		fd = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_redirect(t_fds **fd, t_tokens *list)
{
	t_tokens	*temp;
	int			pipe_no;

	*fd = NULL;
	*fd = malloc(sizeof(t_fds));
	if (!*fd)
		return (EXIT_FAILURE);
	(*fd)->in = NULL;
	(*fd)->out = NULL;
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
