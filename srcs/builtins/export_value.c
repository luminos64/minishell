/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:35:03 by euniceleow        #+#    #+#             */
/*   Updated: 2025/02/20 20:49:24 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//if exits then return 1
int	check_export_exits(t_env *env, char *name)
{
	t_env *temp;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, name, ft_strlen(name)) && temp->name[ft_strlen(name)] == '\0')
			return (1);
		temp = temp->next;
	}
	return (0);
}

// int parse_token(t_tokens *current, char **name, char **value)
// {
// 		int j;

// 	j = 0;
// 	*name = NULL;
// 	*value = NULL;
// 	while (current->str[j] && current->str[j] != '=')
// 		j++;
// 	if (current->str[j] == '=')
// 	{
// 		*name = ft_substr(current->str, 0, j + 1);
// 		*value = ft_strdup(&current->str[j + 1]);
// 		return (1);
// 	}
// 	return (0);
// }

t_env	*export_parser(t_tokens *token)
{
	int		parsed;
	char	*name;
	char	*value;
	t_env	*new_node;

	parsed = parse_token(token, &name, &value);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(name);
		free(value);
		return (NULL);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->print_index = NOT_ASSIGN;
	if(parsed == 1)
		new_node->status = PRINT_EX_ENV;
	else if (parsed == 2)
		new_node->status = PRINT_EX;
	new_node->next = NULL;
	return (new_node);
}

void	update_existing_env_var(t_env *existing_env, t_env *new_env)
{
	free(existing_env->value);
	existing_env->value = new_env->value;
	printf("export updated: `%s' to `%s'\n", existing_env->name, existing_env->value);
	free(new_env->name);
	free(new_env);
}

t_env	*find_env_var(t_env *env, char *name)
{
	while (env)
	{
		if (!ft_strncmp(env->name, name, ft_strlen(name)) && env->name[ft_strlen(name)] == '\0')
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_export_value(t_env **env, t_tokens *tokens)
{
	t_env	*new_env;
	t_env	*existing_env;

	while (tokens)
	{
		if (tokens && !ft_strncmp(tokens->str, "export", 6))
			tokens = tokens->next;
		new_env = export_parser(tokens);
		if (new_env)
		{
			existing_env = find_env_var(*env, new_env->name);
			if (existing_env)
				update_existing_env_var(existing_env, new_env);
			else
			{
				ft_list_addback((void **)env, new_env);
				printf("export added: `%s' `%s'\n", new_env->name, new_env->value);
			}
		}
		else
		{
			printf("export: `%s': not a valid identifier\n", tokens->str);
		}
		tokens = tokens->next;
	}
}
