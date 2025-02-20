/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:30:35 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/19 02:47:03 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtins_main(t_minishell *id, t_tokens *tokens, t_fds *fd)
{
	if (id->pipe > 1 || !id->cmd)
		return (EXIT_FAILURE);
	else if (!ft_strncmp(id->cmd->cmd[0], "cd", 3))
	{
		if (cd_main(id, id->cmd->cmd))
			return (EXIT_FAILURE);
	}
	else if (!ft_strncmp(id->cmd->cmd[0], "export", 7) && id->cmd->cmd[1]) // not flag
		add_export_value(&id->env, tokens);
	else if (!ft_strncmp(id->cmd->cmd[0], "unset", 6) && id->cmd->cmd[1]) // not flag
	{
		printf("unset on process\n");
	}
	else if (!ft_strncmp(id->cmd->cmd[0], "exit", 5))
		ft_exit_main(id, tokens, fd);
	else if (ft_strchr(id->cmd->cmd[0], '=')) // add env -> screen when resrevied in put
	{
		printf("add env on process\n");
	}
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_builtins_child(t_minishell *id, char **cmd)
{
	// if (!ft_strncmp(cmd[0], "./minishell", 12))////////////////////////
	// 	ft_put_error("error", NULL, N_FOUND, W_TITLE);
	if (!ft_strncmp(cmd[0], "echo", 5))
		print_echo(cmd);
	else if (!ft_strncmp(cmd[0], "cd", 3))
		cd_child(cmd);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		printf("%s\n", id->pwd->value);
	else if (!cmd[1] && !ft_strncmp(cmd[0], "unset", 6))
	{
		printf("unset ont active\n");
	}
	else if (!ft_strncmp(cmd[0], "export", 7))
	{
		if (!cmd[1])
			ft_print_export(id->env);
	}
	else if (!ft_strncmp(cmd[0], "env", 4))
		ft_print_env(id->env);
	else if (!ft_strncmp(cmd[0], "exit", 5))
		ft_exit_child(id, cmd);
	else
		return ;
	exit (EXIT_SUCCESS);
}
