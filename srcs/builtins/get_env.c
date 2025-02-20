/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:36:42 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/19 00:24:43 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_env_value(t_env *env, char *envp, int len_name)
{
	int	i;
	int	j;
	int	separate;

	i = 0;
	j = 0;
	separate = 0;
	while (envp[i])
	{
		if (separate == 0)
			env->name[i] = envp[i];
		else
		{
			env->value[j] = envp[i];
			j++;
		}
		if (envp[i] == '=')
			separate = 1;
		i++;
	}
	env->name[len_name + 1] = 0;
	env->value[j] = 0;
	env->status = PRINT_EX_ENV;
	if (!ft_strncmp(env->name, "_=", 3))
		env->status = PRINT_ENV;
}

static int	add_env(t_env **env, char *envp)
{
	t_env	*temp;
	int		len_name;
	int		len;

	len = ft_strlen(envp);
	len_name = ft_strlen_char(envp, '=');
	temp = NULL;
	temp = malloc(sizeof(t_env));
	if (temp)
	{
		temp->name = malloc(sizeof(char) * len_name + 2);
		temp->value = malloc(sizeof(char) * ((len - len_name) + 1));
		temp->next = NULL;
	}
	if (!temp || !temp->name || !temp->value)
	{
		ft_free_env(*env);
		ft_free_env(temp);
		return (EXIT_FAILURE);
	}
	temp->print_index = NOT_ASSIGN;
	add_env_value(temp, envp, len_name);
	ft_list_addback((void **)env, temp);
	return (EXIT_SUCCESS);
}

static void	ft_adj_shlvl(t_minishell *id)
{
	int	n;

	n = ft_atoi(id->shellvl->value);
	free(id->shellvl->value);
	id->shellvl->value = ft_itoa(n + 1);
}

int	ft_getenv(t_minishell *id, t_env **env, char **envp)
{
	t_env	*temp;
	int		i;

	*env = NULL;
	i = 0;
	while (envp[i])
	{
		if (add_env(env, envp[i]))
			return (EXIT_FAILURE);
		i++;
	}
	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "SHLVL=", 7))
			id->shellvl = temp;
		else if (!ft_strncmp(temp->name, "PWD=", 5))
			id->pwd = temp;
		else if (!ft_strncmp(temp->name, "OLDPWD=", 8))
			id->old_pwd = temp;
		temp = temp->next;
	}
	ft_adj_shlvl(id);
	return (EXIT_SUCCESS);
}

void	ft_print_env(t_env *env)
{
	t_env	*temp;
	int		i; ////////////////////////

	temp = env;
	i = 1;
	while (temp)
	{
		if (temp->status > PRINT_EX)
			printf("%2d %s%s\n", i, temp->name, temp->value);
		temp = temp->next;
		i++;
	}
}

