/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:40 by euniceleow        #+#    #+#             */
/*   Updated: 2025/03/05 17:57:32 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c, int ctl)
{
	if (ctl == 0)
	{
		if (c == '<' || c == '>' || c == '|')
			return (1);
	}
	else
	{
		if (c == '<' || c == '>' || c == '|' || c == '\'' || c == '"')
			return (1);
	}
	return (0);
}

static char	*extract_operator(const char *input, int *index)
{
	int		len;
	char	*temp;

	len = 1;
	temp = NULL;
	if (input[*index + 1] == input[*index])
		len++;
	temp = ft_strncpy(&input[*index], len);
	*index += len;
	return (temp);
}

static int	check_quote_pairing(char *str)
{
	int		i;
	int		c;

	i = 0;
	c = 0;
	while (str[i])
	{
		if (str[i] && (str[i] == '"' || str[i] == '\''))
		{
			if (!c)
				c = str[i];
			else if (c == str[i])
				c = 0;
		}
		i++;
	}
	if (!c)
		return (1);
	return (0);
}

static void	tokenize_input(char *str, t_tokens **head, t_minishell *id)
{
	int	i;
	int	j;
	int	ctl;

	i = 0;
	ctl = 0;
	while (str[i])
	{
		while (str[i] && screen_space(str[i]))
			i++;
		j = i;
		if (!str[i])
			break ;
		else if (is_operator(str[i], 0))
			add_node(head, extract_operator(str, &i), &ctl, 0);
		else
		{
			while (str[j] && !is_operator(str[j], 1) && !screen_space(str[i]))
				j++;
			if (str[j] == '\'' || str[j] == '"')
				add_node(head, extrack_token(id, str, &i, i), &ctl, 1);
			else
				add_node(head, extrack_token(id, str, &i, i), &ctl, 0);
		}
	}
}

t_tokens	*check_parser(char *input, t_minishell *id)
{
	t_tokens	*head;

	head = NULL;
	if (!check_quote_pairing(input))
	{
		ft_put_error("The quotes not in pair", NULL, NOT_EXIT, W_TITLE);
		return (NULL);
	}
	else
		tokenize_input(input, &head, id);
	return (head);
}
