/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:12 by usoontra          #+#    #+#             */
/*   Updated: 2025/03/06 00:16:18 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_pwd_prompt(t_minishell *id)
{
	t_env	*temp;
	int		start;
	int		n;

	temp = id->env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "USER=", 6))
			id->user = temp;
		temp = temp->next;
	}
	start = ft_strlen(id->pwd->value) - 1;
	n = 0;
	while (start != 0 && n < 2)
	{
		if (id->pwd->value[start] == '/')
			n++;
		start--;
	}
	return (start);
}

char	*get_prompt(int start, t_minishell *id)
{
	char	*temp;
	char	*temp_user;
	char	*temp_path;
	char	*res;

	temp_user = ft_strjoin("minishell", ": ");
	if (start)
	{
		temp = ft_strjoin(id->pwd->value + start + 1, "$ ");
		temp_path = ft_strjoin("~", temp);
		free(temp);
	}
	else
		temp_path = ft_strjoin(id->pwd->value, "$ ");
	temp = ft_strjoin(RED, temp_user);
	free(temp_user);
	temp_user = ft_strjoin(temp, PINK);
	free(temp);
	temp = ft_strjoin(temp_path, RESET);
	free(temp_path);
	res = ft_strjoin(temp_user, temp);
	free(temp_user);
	free(temp);
	return (res);
}

void	get_input(t_minishell *id)
{
	t_tokens	*tokens;
	char		*line;
	char		*str;

	line = NULL;
	while (1)
	{
		str = get_prompt(len_pwd_prompt(id), id);
		line = readline(str);
		free(str);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		check_signal_line(&id->exit_status);
		tokens = check_parser(line, id);
		add_history(line);
		free(line);
		if (!tokens)
			continue ;
		execution(id, tokens);
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_minishell		id;

	if (ac != 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(av[1], STDERR_FILENO);
		ft_putendl_fd(NO_FILE, STDERR_FILENO);
		return (N_FOUND);
	}
	if (ft_getenv(&id, &id.env, envp))
		return (EXIT_FAILURE);
	id.exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handle_line);
	get_input(&id);
	signal(SIGQUIT, SIG_DFL);
	ft_free_env(id.env);
	return (EXIT_SUCCESS);
}
