/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:12 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/20 21:00:57 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_env(t_env *env)
{
	t_env	*temp;

	temp = NULL;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->value)
			free(env->value);
		temp = env->next;
		free(env);
		env = temp;
	}
	env = NULL;
}

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
	while (n < 2)
	{
		if (id->pwd->value[start] == '/')
			n++;
		start--;
	}
	return (start + 1);
}

char	*get_prompt(int start, t_minishell *id)
{
	char	str[400];
	char	*res;
	int		temp;

	temp = open(".pwd_temp", O_WRONLY | O_CREAT | O_TRUNC, X_PERMIT);
	if (!temp)
		return NULL;
	ft_putstr_fd(RED, temp);
	ft_putstr_fd(id->user->value, temp);
	ft_putstr_fd(": ", temp);
	ft_putstr_fd(PINK, temp);
	ft_putstr_fd("~", temp);
	ft_putstr_fd(id->pwd->value + start, temp);
	ft_putstr_fd("$ ", temp);
	ft_putstr_fd(RESET, temp);
	close(temp);
	temp = open(".pwd_temp", O_RDONLY);
	if (!temp)
		return NULL;
	if (!read(temp, str, 300))
		return NULL;
	close(temp);
	res = ft_strdup(str);
	return (res);
}

void	get_input(t_minishell *id)
{
	t_tokens	*tokens;
	char		*line;
	char		*str;

	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		str = get_prompt(len_pwd_prompt(id), id);
		unlink(".pwd_temp");
		signal(SIGINT, sig_handle_line);
		line = readline(str);
		signal(SIGINT, SIG_IGN);
		// line = readline("$$");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		check_signal(&id->exit_status);
		tokens = check_parser(line, id);
		add_history(line);
		free(line);
		free(str);
		if (!tokens)
			continue;
		execution(id, tokens);
	}
	signal(SIGQUIT, SIG_DFL);
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_minishell		id;

	if (ac != 1)
		ft_put_error(NO_FILE, av[1], N_FOUND, W_TITLE);
	if (ft_getenv(&id, &id.env, envp))
		return (EXIT_FAILURE);
	// id.envp = ft_trans_env(id.env);
	id.exit_status = 0;
	get_input(&id);
	ft_free_env(id.env);
	return (0);
}
