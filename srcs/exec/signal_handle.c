/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:12:42 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/20 20:15:42 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_signal_line(long long *ctl)
{
	static int	i;

	if (*ctl == 7)
		i++;
	else if (i == 0)
		return ;
	else
	{
		i = 0;
		*ctl = EXIT_SIGINT;
	}
}

void	check_signal_heredoc(char *name)
{
	static char	*temp;

	if (name == NULL)
	{
		free(temp);
		ft_child_clear(NULL, NULL, EXIT_SUCCESS);
		exit (EXIT_SIGINT);
	}
	else
		temp = name;
}

void	sig_handle_heredoc(int sig_no)
{
	(void)sig_no;
	printf("\n");
	check_signal_heredoc(NULL);
}

void	sig_handle_exec(int sig_no)
{
	ft_child_clear(NULL, NULL, EXIT_SUCCESS);
	if (sig_no == SIGINT)
	{
		printf("\n");
		exit (EXIT_SIGINT);
	}
	if (sig_no == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		exit (EXIT_SIGQUIT);
	}
}

void	sig_handle_line(int sig_no)
{
	int			n;
	char		*user;
	char		*pwd;
	long long	i;

	(void)sig_no;
	user = getenv("USER");
	pwd = getenv("PWD");
	rl_replace_line("", STDIN_FILENO);
	printf("\n");
	n = 0;
	i = ft_strlen(pwd) - 1;
	while (n < 2)
	{
		if (pwd[i] == '/')
			n++;
		i--;
	}
	printf(RED"%s: ", user);
	printf(PINK"~%s$ "RESET, pwd + i + 1);
	i = 7;
	check_signal_line(&i);
}
