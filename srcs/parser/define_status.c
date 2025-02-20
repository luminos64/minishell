/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 02:55:18 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/06 23:00:12 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	define_status(char *str, int ctl)
{
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	else if (!ft_strncmp(str, "<", 2) || !ft_strncmp(str, ">", 2))
		return (REDIRECT);
	else if (!ft_strncmp(str, "<<", 3) || !ft_strncmp(str, ">>", 3))
		return (REDIRECT);
	else if (!ft_strncmp(str, "$", 1))
		return (EXPAND);
	else if (ctl == 0)
		return (CMD);
	else
		return (CMD_ELEMENT);
}

int	check_status_isredirect(t_tokens **list)
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

// static int	add_tokens(t_tokens **list, char *av, int *ctl)
// {
// 	t_tokens	*temp;

// 	temp = malloc(sizeof(t_tokens));
// 	if (!temp)
// 		return (EXIT_FAILURE);
// 	temp->str = av;
// 	temp->next = NULL;
// 	temp->status = -1;
// 	temp->status = define_status(temp->str, *ctl);
// 	if (!check_status_isredirect(list) && (temp->status == CMD || temp->status == CMD_ELEMENT))
// 		temp->status = REDIRECT_NAME;
// 	if (temp->status == CMD)
// 		(*ctl)++;
// 	if (temp->status == PIPE && ctl > CMD)
// 		(*ctl) = CMD;
// 	ft_list_addback((void **)list, temp);
// 	return (EXIT_SUCCESS);
// }

// static int	add_tokens2(t_tokens **list, t_node *token, int *ctl)
// {
// 	t_tokens	*temp;

// 	temp = malloc(sizeof(t_tokens));
// 	if (!temp)
// 		return (EXIT_FAILURE);
// 	temp->str = token->data;
// 	temp->next = NULL;
// 	temp->status = -1;
// 	temp->status = define_status(temp->str, *ctl);
// 	if (!check_status_isredirect(list) && (temp->status == CMD || temp->status == CMD_ELEMENT))
// 		temp->status = REDIRECT_NAME;
// 	if (temp->status == CMD)
// 		(*ctl)++;
// 	if (temp->status == PIPE && ctl > CMD)
// 		(*ctl) = CMD;
// 	ft_list_addback((void **)list, temp);
// 	return (EXIT_SUCCESS);
// }

// int	to_list2(t_node *tokens, t_tokens **list)
// {
// 	// int		i;
// 	int		ctl;
// 	t_node	*temp;

// 	// i = 0;
// 	ctl = 0;
// 	temp = tokens;
// 	(*list) = NULL;
// 	while (temp)
// 	{
// 		if (add_tokens2(list, temp, &ctl))
// 		{
// 			ft_free_list(*list);
// 			return (EXIT_FAILURE);
// 		}
// 		temp = temp->next;
// 		// i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

// int	to_list(char **av, t_tokens **list)
// {
// 	int		i;
// 	int		ctl;

// 	i = 0;
// 	ctl = 0;
// 	(*list) = NULL;
// 	while (av[i])
// 	{
// 		if (add_tokens(list, av[i], &ctl))
// 		{
// 			ft_free_list(*list);
// 			return (EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }
