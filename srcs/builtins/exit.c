/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:16:12 by usoontra          #+#    #+#             */
/*   Updated: 2025/05/04 18:32:50 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isdigit_str(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ((str[i] >= '0' && str[i] <= '9') || \
str[i] == '-' || str[i] == '+'))
		i++;
	if (str[i])
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_exit_child(t_minishell *id, char **cmd)
{
	int	i;

	i = 0;
	if (cmd[1] && !ft_isdigit_str(cmd[1]))
		i = ft_atoi(cmd[1]);
	if (cmd[1] && i)
	{
		if (cmd[2])
			ft_put_error(": too many arguments", "exit", EXIT_FAILURE, W_TITLE);
		else
		{
			ft_child_clear(NULL, NULL, EXIT_SUCCESS);
			exit (i);
		}
	}
	else if (cmd[1])
	{
		ft_put_error(cmd[1], "exit: ", NOT_EXIT, WO_NEW_LINE);
		ft_put_error(": numeric argument required", NULL, INVALID, WO_TITLE);
	}
	else if (!cmd[1])
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (id->exit_status);
	}
}

static int	ft_exit_screen_2(char *num, int *sign, long long *i, int *j)
{
	*j = 0;
	if (!ft_strncmp(num, "-9223372036854775808", 21))
	{
		*i = 0;
		return (EXIT_SUCCESS);
	}
	while (num[*j] && ((num[*j] > 8 && num[*j] < 14) || num[*j] == 32))
		(*j)++;
	*sign = 1;
	if (num[*j] == '-' || num[*j] == '+')
	{
		if (num[*j] == '-')
			*sign = -1;
		(*j)++;
	}
	return (EXIT_FAILURE);
}

static int	ft_exit_screen(char *num, long long *i)
{
	int					j;
	int					sign;
	unsigned long long	res;

	if (num && !num[0])
		return (EXIT_FAILURE);
	if (num && !ft_isdigit_str(num))
	{
		if (!ft_exit_screen_2(num, &sign, i, &j))
			return (EXIT_SUCCESS);
		res = 0;
		while (num[j] && num[j] >= '0' && num[j] <= '9')
		{
			res = res * 10 + (num[j] - '0');
			if (res > 9223372036854775807)
				return (EXIT_FAILURE);
			j++;
		}
		if (num[j])
			return (EXIT_FAILURE);
		*i = res * sign;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

void	ft_exit_main(t_minishell *id)
{
	long long	i;
	int			ctl;

	printf("exit\n");
	ctl = ft_exit_screen(id->cmd->cmd[1], &i);
	if (id->cmd->cmd[1] && !ctl && id->cmd->cmd[2])
	{
		ft_put_error(": too many arguments", "exit", NOT_EXIT, W_TITLE);
		if (id->exit_status == EXIT_SUCCESS)
			id->exit_status = EXIT_FAILURE;
	}
	else if (id->cmd->cmd[1] && ctl)
	{
		ft_put_error(id->cmd->cmd[1], "exit: ", NOT_EXIT, WO_NEW_LINE);
		ft_put_error(": numeric argument required", NULL, INVALID, WO_TITLE);
	}
	else if (!ctl || !id->cmd->cmd[1])
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		if (!ctl)
			exit (i);
		else
			exit (EXIT_SUCCESS);
	}
}
