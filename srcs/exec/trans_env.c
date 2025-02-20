/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trans_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:00:14 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/19 00:23:45 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trans_env_shlvl(t_env *env, char *res, int *i)
{
	if (!ft_strncmp(env->name, "SHLVL=", 7))
	{
		*i = ft_atoi(env->value);
		free(env->value);
		env->value = ft_itoa(*i);
		// printf("i = %d shlvl = %s	name = %s\n", *i, env->value, env->name);
	}
	*i = ft_strlen(env->name) + ft_strlen(env->value);
	res = malloc(sizeof(char) * (*i + 1));
	return (res);
}

static char	*trans_env_list(t_env *env)
{
	char	*res;
	int		len;
	int		i;
	int		j;

	res = NULL;
	res = trans_env_shlvl(env, res, &len);
	if (!res)
		return NULL;
	i = 0;
	j = 0;
	while (len)
	{
		if (env->name[i])
			res[i] = env->name[i];
		else
		{
			res[i] = env->value[j];
			j++;
		}
		i++;
		len--;
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
	res = malloc(sizeof(char *) * i);
	if (!res)
		return NULL;
	temp = list;
	i = 0;
	while (temp)
	{
		if (temp->status > 1)
		{
			res[i] = trans_env_list(temp);
			if(!res[i])
				return NULL;
			i++;
		}
		temp = temp->next;
	}
	res[i] = 0;
	return (res);
}
