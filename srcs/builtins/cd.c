/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:50:35 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 02:05:24 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_main_2_screen(t_minishell *id, char **cmd)
{
	if (!cmd[1] || !ft_strncmp(cmd[1], "--", 3))
	{
		if (id->home->status == NOT_ASSIGN)
			return (EXIT_FAILURE);
		chdir(id->home->value);
	}
	else if (!ft_strncmp(cmd[1], "-", 2) && !cmd[2])
	{
		if (id->old_pwd->status == NOT_ASSIGN)
			return (EXIT_FAILURE);
		printf("%s\n", id->old_pwd->value);
		chdir(id->old_pwd->value);
	}
	else if (cmd[2])
		return (EXIT_FAILURE);
	else
		return (INVALID);
	return (EXIT_SUCCESS);
}

int	cd_main(t_minishell *id, char **cmd)
{
	char	path[100];
	int		indicator;
	DIR		*dr;

	dr = NULL;
	indicator = cd_main_2_screen(id, cmd);
	if (indicator == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (indicator)
	{
		dr = opendir(cmd[1]);
		if (!dr)
			return (EXIT_FAILURE);
		closedir(dr);
		chdir(cmd[1]);
	}
	getcwd(path, 100);
	free(id->old_pwd->value);
	id->old_pwd->value = id->pwd->value;
	id->pwd->value = ft_strdup(path);
	id->old_pwd->status = PRINT_EX_ENV;
	id->exit_status = 0;
	return (EXIT_SUCCESS);
}

static int	cd_child_2_screen(t_minishell *id, char **cmd)
{
	if (!cmd[1] || !ft_strncmp(cmd[1], "--", 3))
	{
		if (id->home->status == NOT_ASSIGN)
			ft_put_error("HOME not set", "cd: ", EXIT_FAILURE, W_TITLE);
	}
	else if (!ft_strncmp(cmd[1], "-", 2) && !cmd[2])
	{
		if (id->old_pwd->status == NOT_ASSIGN)
			ft_put_error("OLDPWD not set", "cd: ", EXIT_FAILURE, W_TITLE);
		printf("%s\n", id->old_pwd->value);
	}
	else if (cmd[2])
		ft_put_error("too many arguments", "cd: ", EXIT_FAILURE, W_TITLE);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	cd_child(t_minishell *id, char **cmd)
{
	DIR		*dr;

	dr = NULL;
	if (!cd_child_2_screen(id, cmd))
		return ;
	dr = opendir(cmd[1]);
	if (dr)
		closedir(dr);
	else if (cmd[2] || !dr)
	{
		ft_put_error("cd: ", NULL, NOT_EXIT, WO_NEW_LINE);
		if (cmd[1] && cmd[1][0] == '-')
			ft_put_error(": invalid option", cmd[1], INVALID, WO_TITLE);
		else if (!dr)
		{
			if (access(cmd[1], F_OK) < 0)
				ft_put_error(NO_FILE, cmd[1], EXIT_FAILURE, WO_TITLE);
			else if (errno == ENOTDIR)
				ft_put_error(NOT_DIR, cmd[1], EXIT_FAILURE, WO_TITLE);
			else if (access(cmd[1], X_OK) < 0)
				ft_put_error(NO_PER, cmd[1], EXIT_FAILURE, WO_TITLE);
		}
	}
}
