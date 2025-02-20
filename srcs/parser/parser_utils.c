/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:48 by euniceleow        #+#    #+#             */
/*   Updated: 2025/02/19 00:24:38 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

char *ft_strncpy(const char *src, int len)
{
	int		i;
	char	*dest;

	i = -1;
	dest = malloc(len + 1);
	if (!dest)
		return NULL;
	while (i++ < len)
		dest[i] = src[i];
	dest[len] = '\0';
	return (dest);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*get_env_value(t_minishell *id, const char *var_name)
{
	t_env	*temp;
	char	*str;
	int		len;

	if (!ft_strncmp(var_name, "?", 1))
	{
		str = ft_strjoin(ft_itoa(id->exit_status), var_name + 1);
		if (!str)
			return NULL;
		return (str);
	}
	temp = id->env;
	while (temp)
	{
		len = ft_strlen(temp->name);
		if (!ft_strncmp(var_name, temp->name, len - 1))
			return (temp->value);
		temp = temp->next;
	}
	return NULL;
}
