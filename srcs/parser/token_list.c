/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:28 by euniceleow        #+#    #+#             */
/*   Updated: 2025/02/17 12:45:00 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*create_node(const char *token)
{
	t_tokens *node;
	int len = 0;

	while (token[len])
		len++;
	node = malloc(sizeof(t_tokens));
	if (!node)
		return NULL;
	node->str = ft_strncpy(token, len);
	if (!node->str)
	{
		free(node);
		return NULL;
	}
	node->status = -1;
	node->next = NULL;
	return (node);
}

void	add_node(t_tokens **list, char *token, int *ctl)
{
	t_tokens *temp;

	temp  = create_node(token);
	if (!temp)
		return;
	temp->str = token;
	temp->next = NULL;
	temp->status = define_status(temp->str, *ctl);
	if (!check_status_isredirect(list) && temp->status != REDIRECT)
		temp->status = REDIRECT_NAME;
	if (temp->status == CMD)
		(*ctl)++;
	if (temp->status == PIPE && ctl > CMD)
		(*ctl) = CMD;
	ft_list_addback((void **)list, temp);
}

// void print_linked_list(t_tokens *head)
// {
//     int i = 0;
//     printf("Parsed Tokens:\n");

//     while (head)
//     {
//         printf("Token %d: %s (Status: %d)\n", i, head->str, head->status);
//         head = head->next;
//         i++;
//     }
// }

void	free_tokens_list(t_tokens *head)
{
	t_tokens *temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->str);
		free(temp);
	}
}
