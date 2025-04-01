/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:57 by euniceleow        #+#    #+#             */
/*   Updated: 2025/03/10 20:12:20 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strncpy(const char *src, int len)
{
	int		i;
	char	*dest;

	i = 0;
	if (!len)
		return (NULL);
	dest = NULL;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	while (i < len && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	get_env_find(char *var_name, t_env *temp, int var_len)
{
	int	len;

	len = ft_strlen(temp->name);
	if (temp->name[ft_strlen(temp->name) - 1] == '=')
		len--;
	if (var_len > len)
		len = var_len;
	if (!ft_strncmp(var_name, temp->name, len) && temp->status > -1)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

char	*get_env_value(t_minishell *id, const char *var_name)
{
	t_env	*temp;
	char	*str;
	char	*i;
	int		var_len;

	if (!ft_strncmp(var_name, "?", 1))
	{
		i = ft_itoa(id->exit_status);
		str = ft_strjoin(i, var_name + 1);
		free(i);
		if (!str)
			return (NULL);
		return (str);
	}
	temp = id->env;
	var_len = ft_strlen(var_name);
	if (var_name[ft_strlen(var_name) - 1] == '=')
		var_len--;
	while (temp)
	{
		if (!get_env_find((char *)var_name, temp, var_len))
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	return (NULL);
}

static char	*token_2_home(t_minishell *id, char *str, int start, char *temp)
{
	char	*res;

	if (!temp)
		return (NULL);
	res = NULL;
	if (str[start] == '~' && (str[start + 1] == '/' || \
			str[start + 1] == ' ' || str[start + 1] == '\0'))
	{
		res = ft_strjoin3_a(NULL, get_env_value(id, "ZDOTDIR"), \
			ft_strdup(temp + 1));
	}
	else
		res = ft_strdup(temp);
	free(temp);
	return (res);
}

char	*extrack_token(t_minishell *id, char *str, int *len, int i)
{
	char	*temp;
	char	*res;
	int		start;
	int		no_quote;

	temp = NULL;
	no_quote = 0;
	start = i;
	while (str[i] && !screen_space(str[i]) && !is_operator(str[i], 1))
		i++;
	if (str[i] == '\'' || str[i] == '"')
		no_quote++;
	temp = ft_expand_line(ft_strncpy(str + start, i - start), id, temp);
	res = NULL;
	if (no_quote)
		res = ft_qoute_screen(id, str, &i, temp);
	else
		res = token_2_home(id, str, start, temp);
	*len = i;
	return (res);
}
