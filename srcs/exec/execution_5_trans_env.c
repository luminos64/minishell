/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_5_trans_env.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:00:14 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/05 17:22:21 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trans_env_list(t_env *env)
{
	char	*res;
	int		i;
	int		j;

	res = NULL;
	i = ft_strlen(env->name) + ft_strlen(env->value);
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (env->name[i])
	{
		res[i] = env->name[i];
		i++;
	}
	j = 0;
	while (env->value[j])
	{
		res[i] = env->value[j];
		j++;
		i++;
	}
	res[i] = 0;
	return (res);
}

char	**trans_env(t_env *list)
{
	t_env	*temp;
	char	**res;
	int		i;

	res = NULL;
	i = ft_list_count(list);
	res = malloc(sizeof(char *) * (i + 1));
	if (!res)
		return (NULL);
	temp = list;
	i = 0;
	while (temp)
	{
		if (temp->status > 1)
		{
			res[i] = trans_env_list(temp);
			if (!res[i])
				return (NULL);
			i++;
		}
		temp = temp->next;
	}
	res[i] = 0;
	return (res);
}
