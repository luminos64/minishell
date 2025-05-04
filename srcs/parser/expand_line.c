/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 01:42:40 by usoontra          #+#    #+#             */
/*   Updated: 2025/05/04 18:31:22 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_3_getval(t_minishell *id, char *str, char **head, int len)
{
	char	*value;
	char	*temp;
	int		start;

	value = NULL;
	temp = NULL;
	start = len;
	while (str[len] && (str[len + 1] != '\0' && str[len + 1] != ' ' && \
str[len + 1] != '$' && str[len + 1] != '\'' && str[len + 1] != '/'))
		len++;
	if (len == start)
		return (len);
	temp = ft_strncpy(str + start + 1, len - start);
	value = get_env_value(id, temp);
	free(temp);
	temp = NULL;
	temp = ft_strjoin(*head, value);
	free(value);
	free(*head);
	if (str[len])
		len++;
	*head = temp;
	return (len);
}

static char	*expand_2_head(char *str, char *head, int len, int start)
{
	char	*res;
	char	*temp;

	if (len == start)
		return (head);
	if (!start)
	{
		temp = ft_strncpy(str, len);
		return (temp);
	}
	temp = ft_strncpy(str + start, len - start);
	if (!temp)
		return (NULL);
	if (!head)
		return (temp);
	res = NULL;
	res = ft_strjoin(head, temp);
	free(head);
	free(temp);
	return (res);
}

char	*ft_expand_line(char *str, t_minishell *id, char *res)
{
	int		i;
	int		end;

	if (!str || !str[0])
	{
		free(str);
		return (ft_strdup(""));
	}
	i = 0;
	end = 0;
	res = NULL;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			res = expand_2_head(str, res, i, end);
			end = expand_3_getval(id, str, &res, i);
		}
		i++;
	}
	if (!res || end)
		res = expand_2_head(str, res, i, end);
	free(str);
	return (res);
}
