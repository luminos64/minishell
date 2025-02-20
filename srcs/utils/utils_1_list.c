/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:40:02 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/17 22:01:54 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_list_addback(void **list, void *new)
{
	t_env	*last;

	if (!*list)
		*list = new;
	else
	{
		last = *list;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

int	ft_list_count(void *list)
{
	t_env	*temp;
	int		len;

	temp = (t_env *)list;
	len = 0;
	while (temp)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}

void	ft_print_list(void *list, char *str, int ctl)
{
	t_isredirect	*temp;
	int				i;

	i = 1;
	temp = (t_isredirect *)list;
	printf("===========	%s: print list		===========\n", str);
	while (temp)
	{
		printf("%2d %s %s", i, str, temp->name);
		printf("\t\tstatus = %d", temp->type);
		if (ctl == 1)
			printf("\t\tindex = %d", temp->index);
		printf("\n");
		temp = temp->next;
		i++;
	}
	printf("====================================================\n");
}

void	ft_free_list(void *list)
{
	t_tokens	*temp;
	t_tokens	*temp2;

	temp = (t_tokens *)list;
	while (temp)
	{
		temp2 = temp->next;
		free(temp);
		temp = temp2;
	}
	list = NULL;
}
