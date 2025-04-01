/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:30:35 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 20:12:05 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_echo(char **cmd)
{
	int	i;
	int	j;
	int	nline;

	i = 1;
	nline = 1;
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 1;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] != '\0')
			break ;
		nline = 0;
		i++;
	}
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], 1);
		if (cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (nline)
		ft_putstr_fd("\n", 1);
}

int	ft_builtins_main(t_minishell *id, t_redirect *fd)
{
	ft_child_clear(id, fd, EXIT_FAILURE);
	if (id->pipe > 1 || !id->cmd)
		return (EXIT_FAILURE);
	else if (!ft_strncmp(id->cmd->cmd[0], "cd", 3))
	{
		if (cd_main(id, id->cmd->cmd))
			return (EXIT_FAILURE);
	}
	else if (!ft_strncmp(id->cmd->cmd[0], "export", 7) && id->cmd->cmd[1])
		add_export_value(id, &id->env, id->cmd->cmd);
	else if (!ft_strncmp(id->cmd->cmd[0], "unset", 6) && id->cmd->cmd[1])
		ft_unset(id, &id->env, id->cmd->cmd);
	else if (!ft_strncmp(id->cmd->cmd[0], "exit", 5))
		ft_exit_main(id);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_builtins_child(t_minishell *id, char **cmd)
{
	if (!ft_strncmp(cmd[0], "echo", 5))
		print_echo(cmd);
	else if (!ft_strncmp(cmd[0], "cd", 3))
		cd_child(id, cmd);
	else if (!ft_strncmp(cmd[0], "pwd", 4))
		printf("%s\n", id->pwd->value);
	else if (!ft_strncmp(cmd[0], "unset", 6))
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (EXIT_SUCCESS);
	}
	else if (!ft_strncmp(cmd[0], "export", 7))
	{
		if (!cmd[1])
			ft_print_export(id->env);
	}
	else if (!ft_strncmp(cmd[0], "env", 4))
		ft_print_env(id->env, cmd);
	else if (!ft_strncmp(cmd[0], "exit", 5))
		ft_exit_child(id, cmd);
	else
		return ;
	ft_child_clear(NULL, NULL, EXIT_SUCCESS);
	exit (EXIT_SUCCESS);
}

int	check_correct_name(char *str, int *len)
{
	int	i;

	i = 0;
	if (str[i] != '_' && !ft_isalpha(str[i]))
		return (EXIT_FAILURE);
	i++;
	while (str[i])
	{
		if (str[i] == '=')
		{
			i++;
			break ;
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (EXIT_FAILURE);
		i++;
	}
	*len = i;
	return (EXIT_SUCCESS);
}
