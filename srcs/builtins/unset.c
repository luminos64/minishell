/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 22:46:11 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/20 22:46:16 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_env_node_check_use(t_env *current)
{
	if (!ft_strncmp(current->name, "PWD=", 5))
		current->status = NOT_ASSIGN;
	else if (!ft_strncmp(current->name, "OLDPWD=", 8))
		current->status = NOT_ASSIGN;
	else if (!ft_strncmp(current->name, "HOME=", 6))
		current->status = NOT_ASSIGN;
	else if (!ft_strncmp(current->name, "USER=", 6))
		current->status = NOT_ASSIGN;
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	remove_env_node(t_env **env, char *name)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = *env;
	while (current != NULL)
	{
		if (!ft_strncmp(current->name, name, ft_strlen(name)))
		{
			if (!remove_env_node_check_use(current))
				return ;
			if (prev == NULL)
				*env = current->next;
			else
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	ft_unset(t_minishell *id, t_env **env, char **names)
{
	int		i;

	i = 1;
	while (names[i])
	{
		remove_env_node(env, names[i]);
		i++;
	}
	id->exit_status = 0;
}
