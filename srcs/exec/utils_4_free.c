/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:44:50 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 20:01:49 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_list(void *list)
{
	t_tokens	*temp;
	t_tokens	*temp2;

	temp = (t_tokens *)list;
	while (temp)
	{
		temp2 = temp->next;
		free(temp->str);
		free(temp);
		temp = temp2;
	}
	list = NULL;
}

void	ft_free_split(char **cmd)
{
	int		i;

	i = 0;
	while (cmd && cmd[i])
	{
		free(cmd[i]);
		cmd[i] = NULL;
		i++;
	}
	if (cmd)
		free(cmd);
	cmd = NULL;
}

void	ft_free_exec(t_minishell *id, t_redirect *fd)
{
	t_iscmd	*run;
	t_iscmd	*temp;

	run = id->cmd;
	while (run)
	{
		temp = run->next;
		ft_free_split(run->cmd);
		free(run);
		run = temp;
	}
	if (fd)
		ft_free_list(fd);
}

void	ft_free_env(t_env *env)
{
	t_env	*temp;

	temp = NULL;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp = env->next;
		free(env);
		env = temp;
	}
	env = NULL;
}

void	ft_child_clear(t_minishell *id, t_redirect *fd, int ctl)
{
	static t_minishell	*temp_id;
	static t_redirect	*temp_fd;

	if (ctl == EXIT_FAILURE)
	{
		temp_id = id;
		temp_fd = fd;
	}
	else
	{
		ft_free_exec(temp_id, temp_fd);
		ft_free_env(temp_id->env);
	}
}
