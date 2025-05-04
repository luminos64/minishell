/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:35:03 by euniceleow        #+#    #+#             */
/*   Updated: 2025/03/06 00:32:44 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_token(char *str, char **name, char **value, int *parsed)
{
	int	name_len;
	int	len;

	if (check_correct_name(str, &name_len))
	{
		ft_put_error(str, "export: `", NOT_EXIT, WO_NEW_LINE);
		ft_put_error(": not a valid identifier", "\'", NOT_EXIT, WO_TITLE);
		return (EXIT_FAILURE);
	}
	if (str[name_len - 1] == '=')
		*parsed = PRINT_EX_ENV;
	else
		*parsed = PRINT_EX;
	len = ft_strlen(str) - name_len;
	*name = ft_substr(str, 0, name_len);
	if (!*name)
		return (EXIT_FAILURE);
	*value = ft_substr(str, name_len, len);
	if (!*value)
	{
		free(*name);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void	update_env_export(t_env *existing_env, char *name, char *value)
{
	int	len;

	len = ft_strlen(name);
	if (name[len - 1] != '=')
	{
		existing_env->status = PRINT_EX;
		if (existing_env->name[ft_strlen(existing_env->name) - 1] == '=')
			existing_env->status = PRINT_EX_ENV;
		free(name);
		free(value);
		return ;
	}
	existing_env->status = PRINT_EX_ENV;
	free(existing_env->name);
	free(existing_env->value);
	existing_env->name = name;
	existing_env->value = value;
}

static t_env	*find_env_var(t_env *env, char *name)
{
	int		len;
	int		adj;
	int		env_len;
	t_env	*temp;

	adj = 0;
	len = ft_strlen(name);
	if (name[len - 1] != '=')
		adj = -1;
	temp = env;
	while (temp)
	{
		env_len = ft_strlen(temp->name);
		if (temp->name[env_len - 1] != '=')
		{
			if (!ft_strncmp(temp->name, name, len + adj))
				return (temp);
		}
		if (!ft_strncmp(temp->name, name, env_len + adj))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

static void	add_new_env(t_env **env, char *name, char *value, int parsed)
{
	t_env	*new;

	new = NULL;
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->name = name;
	new->value = value;
	new->status = parsed;
	new->print_index = NOT_ASSIGN;
	new->next = NULL;
	ft_list_addback((void **)env, new);
}

void	add_export_value(t_minishell *id, t_env **env, char **cmd)///exit status
{
	int		i;
	int		parsed;
	char	*name;
	char	*value;
	t_env	*existing_env;

	i = 1;
	name = NULL;
	value = NULL;
	existing_env = NULL;
	while (cmd[i])
	{
		if (parse_token(cmd[i], &name, &value, &parsed))
			id->exit_status = 1;
		else
		{
			existing_env = find_env_var(*env, name);
			if (existing_env)
				update_env_export(existing_env, name, value);
			else
				add_new_env(env, name, value, parsed);
			id->exit_status = 0;
		}
		i++;
	}
}
