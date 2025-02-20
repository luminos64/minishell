/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:12:42 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/17 22:23:11 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_signal(int *ctl)
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

void	sig_handle_heredoc(int sig_no)
{
	(void)sig_no;
	printf("\n");
	exit (EXIT_SIGINT);
}

void	sig_handle_exec(int sig_no)
{
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
	char	*user;
	char	*pwd;
	int		i;
	int		n;

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
	check_signal(&i);
}
