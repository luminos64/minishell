/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3_put_error.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:28:18 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/10 19:19:17 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_path(char *cmd)
{
	DIR	*dr;

	dr = NULL;
	dr = opendir(cmd);
	if (dr)
	{
		closedir(dr);
		ft_put_error(": Is a directory", ft_strdup(cmd), N_PERMIT, W_TITLE);
	}
	else if (!access(cmd, X_OK))
		return (cmd);
	else if (!access(cmd, F_OK))
		ft_put_error(NO_PER, ft_strdup(cmd), N_PERMIT, W_TITLE);
	else
		ft_put_error(NO_FILE, cmd, N_FOUND, W_TITLE);
	return (cmd);
}

void	ft_put_error(char *str, char *cmd, int exit_code, int ctl)
{
	int	i;

	i = 0;
	if (ctl != WO_TITLE)
		write(STDERR_FILENO, "minishell: ", 11);
	while (cmd && cmd[i])
	{
		write(STDERR_FILENO, &cmd[i], 1);
		i++;
	}
	i = 0;
	while (str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	if (ctl != WO_NEW_LINE)
		write(STDERR_FILENO, "\n", 1);
	if (exit_code == N_PERMIT)
		free(cmd);
	if (exit_code)
	{
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (exit_code);
	}
}

void	ft_put_fd_error(char *str, char *name, int *fd)
{
	int	i;

	i = 0;
	close(fd[PIPE_WRITE]);
	write(STDERR_FILENO, "minishell: ", 11);
	while (name[i])
	{
		write(STDERR_FILENO, &name[i], 1);
		i++;
	}
	i = 0;
	while (str[i])
	{
		write(STDERR_FILENO, &str[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	ft_child_clear(NULL, NULL, EXIT_SUCCESS);
	exit (EXIT_FAILURE);
}
