/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:59:47 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/20 20:51:25 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_findmin(t_env *env)
{
	t_env	*run;
	char	*str;
	int		min;
	int		cmp;

	run = env;
	min = INT_MIN;
	str = NULL;
	while (run)
	{
		cmp = ft_strncmp("~", run->name, -1);
		if (cmp > min && run->print_index == -1)
		{
			min = cmp;
			str = run->name;
		}
		run = run->next;
	}
	return (str);
}

static void	ft_addindex(t_env *env, int len)
{
	t_env	*temp;
	char	*str;
	int		i;

	len = ft_list_count(env);
	i = 1;
	while (i <= len)
	{
		str = ft_findmin(env);
		temp = env;
		while (temp)
		{
			if (ft_strncmp(str, temp->name, -1) == 0)
			{
				temp->print_index = i;
				i++;
				break ;
			}
			temp = temp->next;
		}
	}
}

void	ft_print_export(t_env *env)
{
	t_env	*temp;
	int		len;
	int		i; //////////////////////////////

	len = ft_list_count(env);
	ft_addindex(env, len);
	i = 1;
	while (i <= len)
	{
		temp = env;
		while (temp)
		{
			if (temp->print_index == i)
			{
				if (temp->status > CMD && temp->status < REDIRECT)
				{
					// if(temp->status ==  PRINT_EX && temp->value == NULL)
					if(temp->status ==  PRINT_EX)
						printf("%2d declare -x %s\n", i, temp->name);
					else if (temp->status ==  PRINT_EX_ENV)
						printf("%2d declare -x %s\"%s\"\n", i, temp->name, temp->value);
				}
				i++;
				temp->print_index = NOT_ASSIGN;
				break ;
			}
			temp = temp->next;
		}
	}
}
