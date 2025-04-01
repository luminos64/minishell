/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:28 by euniceleow        #+#    #+#             */
/*   Updated: 2025/04/01 20:40:54 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	screen_space(char c)
{
	if (c == ' ' || c == '\t')
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	define_status(char *str, int ctl, int quote)
{
	if (!ft_strncmp(str, "|", 2) && !quote)
		return (PIPE);
	else if (!quote && (!ft_strncmp(str, "<", 2) || !ft_strncmp(str, ">", 2)))
		return (REDIRECT);
	else if (!quote && (!ft_strncmp(str, "<<", 3) || !ft_strncmp(str, ">>", 3)))
		return (REDIRECT);
	else if (ctl == 0)
		return (CMD);
	else
		return (CMD_ELEMENT);
}

static int	check_status_isredirect(t_tokens **list)
{
	t_tokens	*temp;

	if (!*list)
		return (EXIT_FAILURE);
	temp = *list;
	while (temp->next)
		temp = temp->next;
	if (temp->status == REDIRECT)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

void	add_node(t_tokens **list, char *token, int *ctl, int quote)
{
	t_tokens	*temp;

	if (!token)
		return ;
	temp = malloc(sizeof(t_tokens));
	if (!temp)
		return ;
	temp->str = token;
	temp->next = NULL;
	temp->status = define_status(temp->str, *ctl, quote);
	if (!check_status_isredirect(list) && \
			temp->status != REDIRECT && temp->status != PIPE)
		temp->status = REDIRECT_NAME;
	if (temp->status == CMD)
		(*ctl)++;
	if (temp->status == PIPE && *ctl > CMD)
		(*ctl) = CMD;
	ft_list_addback((void **)list, temp);
}
