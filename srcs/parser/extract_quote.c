/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 04:40:10 by usoontra          #+#    #+#             */
/*   Updated: 2025/05/04 18:32:30 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_quote_part_adj_len(char *str, int *i, int quote, int *q)
{
	int	len;

	len = *i;
	if (!str[*i])
		return (0);
	while (*q % 2 != 0 && str[*i] && str[*i] != quote)
	{
		(*i)++;
	}
	len = *i - len;
	if (*q % 2 != 0 && str[*i] == quote)
	{
		(*q)++;
		(*i)++;
	}
	return (len);
}

static char	*ft_quote_part_extract(char *str, char *head, int end)
{
	char	*res;
	char	*temp;

	if (end == 0)
		return (ft_strdup(""));
	temp = NULL;
	temp = ft_strncpy(str, end);
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

static char	*ft_quote_join_head(char *head, char *temp)
{
	char	*res;

	if (!head && !temp)
		return (NULL);
	else if (!head)
		return (temp);
	else if (!temp)
		return (head);
	res = NULL;
	res = ft_strjoin(head, temp);
	free(head);
	free(temp);
	return (res);
}

static char	*ft_quote_part(t_minishell *id, char *str, int *i, char *head)
{
	char	*res;
	int		quote[2];
	int		start;
	int		end;

	res = NULL;
	quote[0] = str[*i];
	quote[1] = 0;
	while ((str[*i] && str[*i] != ' '))
	{
		while (str[*i] && str[*i] == quote[0])
		{
			quote[1]++;
			(*i)++;
		}
		start = *i;
		end = ft_quote_part_adj_len(str, i, quote[0], &quote[1]);
		res = ft_quote_part_extract(str + start, res, end);
		if (quote[1] % 2 == 0)
			break ;
	}
	if (quote[0] == '"')
		res = ft_expand_line(res, id, res);
	res = ft_quote_join_head(head, res);
	return (res);
}

char	*ft_qoute_screen(t_minishell *id, char *str, int *i, char *head)
{
	char	*temp;
	char	*res;
	int		start;

	res = NULL;
	temp = NULL;
	while (str[*i] && ((str[*i] == '"' || str[*i] == '\'')))
	{
		temp = ft_quote_part(id, str, i, temp);
		start = *i;
		while (str[*i] && str[*i] != ' ' && !is_operator(str[*i], 1))
			(*i)++;
		temp = ft_quote_join_head(temp, \
ft_expand_line(ft_strncpy(str + start, *i - start), id, temp));
	}
	if (!ft_strncmp(head, "$", 2))
	{
		free(head);
		head = NULL;
	}
	res = ft_strjoin(head, temp);
	if (head)
		free(head);
	free(temp);
	return (res);
}
