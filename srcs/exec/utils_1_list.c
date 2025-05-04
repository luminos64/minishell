/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:40:02 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 20:04:59 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

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
	t_redirect	*temp;
	int			i;

	i = 1;
	temp = (t_redirect *)list;
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

void	ft_print_cmd(t_iscmd *cmd, char *str)
{
	t_iscmd	*temp;
	int		i;

	temp = cmd;
	printf("===========	%s: print cmd		===========\n", str);
	while (temp)
	{
		i = 0;
		printf("======	cmd index %d	======================\n", temp->index);
		while (temp->cmd[i])
		{
			printf("cmd = %s\n", temp->cmd[i]);
			i++;
		}
		temp = temp->next;
	}
	printf("====================================================\n");
}
