/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:16:12 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/18 19:45:32 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i])
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_exit_child(t_minishell *id, char **cmd)
{
	if(cmd[1] && !ft_isdigit_str(cmd[1]))
	{
		if (cmd[2])
			ft_put_error(": too many arguments", "exit", EXIT_FAILURE, W_TITLE);
		else
			exit (ft_atoi(cmd[1]));
	}
	else if (cmd[1])
	{
		ft_put_error(cmd[1], "exit: ", NOT_EXIT, WO_NEW_LINE);
		ft_put_error(": numeric argument required", NULL, INVALID, WO_TITLE);
	}
	else if (!cmd[1])
		exit (id->exit_status);
}

void	ft_exit_main(t_minishell *id, t_tokens *tokens, t_fds *fd)
{
	printf("exit\n");
	if(id->cmd->cmd[1] && !ft_isdigit_str(id->cmd->cmd[1]) && id->cmd->cmd[2])
	{
		ft_put_error(": too many arguments", "exit", NOT_EXIT, W_TITLE);
		if (id->exit_status == EXIT_SUCCESS)
			id->exit_status = EXIT_FAILURE;
	}
	ft_free_exec(tokens, id, fd);
	ft_free_env(id->env);
	if(id->cmd->cmd[1] && !ft_isdigit_str(id->cmd->cmd[1]))
		exit (ft_atoi(id->cmd->cmd[1]));
	else if (id->cmd->cmd[1])
	{
		ft_put_error(id->cmd->cmd[1], "exit: ", NOT_EXIT, WO_NEW_LINE);
		ft_put_error(": numeric argument required", NULL, INVALID, WO_TITLE);
	}
	else
		exit (EXIT_SUCCESS);
}
