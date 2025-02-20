/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:51:05 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/18 21:08:26 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_char(char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count + 1);
}

static char	*add_split_path(char *str, int *start)
{
	char	*res;
	int		len;
	int		i;

	res = NULL;
	len = ft_strlen_char(str + *start, ':');
	res = malloc(sizeof(char *) * (len + 2));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[*start + i];
		i++;
	}
	res[i] = '/';
	res[i + 1] = 0;
	*start += i + 1;
	return (res);
}

static char	**ft_split_path(char *str, char c)
{
	char	**res;
	int		start;
	int		len;
	int		i;

	len = ft_count_char(str, c);
	res = NULL;
	res = malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	start = 0;
	while (len)
	{
		res[i] = add_split_path(str, &start);
		if (!res[i])
		{
			ft_free_split(res);
			return (NULL);
		}
		i++;
		len--;
	}
	res[i] = NULL;
	return (res);
}

static char	*get_path_2_join(char **all_path, char **cmd)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while (all_path[i])
	{
		path = ft_strjoin(all_path[i], *cmd);
		if (!path)
			return (*cmd);
		if (!access(path, F_OK))
		{
			ft_free_split(all_path);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_split(all_path);
	return (*cmd);
}

char	*get_path(t_env *env, char **cmd)
{
	t_env	*temp;
	char	**all_path;
	char	*path;

	if (ft_strchr(*cmd, '/'))
	{
		if (!access(*cmd, F_OK))
			return (*cmd);
		else
			ft_put_error(NO_FILE, *cmd, N_FOUND, W_TITLE);
	}
	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "PATH=", 6))
		{
			all_path = ft_split_path(temp->value, ':');
			if (!all_path)
				return (*cmd);
			path = get_path_2_join(all_path, cmd);
			return (path);
		}
		temp = temp->next;
	}
	return (*cmd);
}
