/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:50:35 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/20 20:44:13 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_main(t_minishell *id, char	**cmd)
{
	char	path[100];
	char	*str;
	DIR		*dr;

	str = NULL;
	if (!cmd[1])////////// cd ~
	{
		str = get_env_value(id, "HOME");
		chdir(str);
	}
	else if (cmd[1][0] == '-' && !cmd[1][1])
	{
		str = get_env_value(id, "OLDPWD");
		chdir(str);
	}
	else if (cmd[1][0] == '-' || cmd[2])
		return (EXIT_FAILURE);
	else
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
	return (EXIT_SUCCESS);
}

void	cd_child(char **cmd)
{
	DIR		*dr;

	if (!cmd[1])
		return ;
	dr = opendir(cmd[1]);
	if (dr)
		closedir(dr);
	ft_put_error("cd: ", NULL, NOT_EXIT, WO_NEW_LINE);
	if (cmd[1][0] == '-')
		ft_put_error(": invalid option", cmd[1], INVALID, WO_TITLE);
	else if (cmd[2])
		ft_put_error("too many arguments", NULL, EXIT_FAILURE, WO_TITLE);
	else if (!dr)
	{
		if (access(cmd[1], F_OK) < 0)
			ft_put_error(NO_FILE, cmd[1], EXIT_FAILURE, WO_TITLE);
		else if (errno == ENOTDIR)
			ft_put_error(": Not a directory", cmd[1], EXIT_FAILURE, WO_TITLE);
		else if (access(cmd[1], X_OK) < 0)
			ft_put_error(NO_PER, cmd[1], EXIT_FAILURE, WO_TITLE);
	}
}
