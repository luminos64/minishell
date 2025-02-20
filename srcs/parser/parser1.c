/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:40 by euniceleow        #+#    #+#             */
/*   Updated: 2025/02/17 12:37:53 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(const char *input)
{
	int i;
	int word;

	i = 0;
	word = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (is_operator(input[i]))
		{
			if (input[i + 1] == input[i])
				i++;
			i++;
			word++;
		}
		else
		{
			while (input[i] && !is_operator(input[i]) && input[i] != ' ')
				i++;
			word++;
		}
	}
	return (word);
}

int	get_token_len(const char *input)
{
    int i;

    i = 0;
    if (is_operator(input[i]))
    {
        if (input[i + 1] == input[i])
            return 2;
        return 1;
    }
    while (input[i] && !is_operator(input[i]) && input[i] != ' ' && input[i] != '\t')
        i++;
    return (i);
}

void	tokenize_input(char *input, t_tokens **head, t_minishell *id)
{
    int i;
    int		ctl;

    i = 0;
    ctl = 0;
    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        if (!input[i])
            break;
        if (input[i] == '\'' || input[i] == '"')
            add_node(head, extract_quoted_string(input, &i), &ctl);
        else if (is_operator(input[i]))
            add_node(head, extract_operator(input, &i), &ctl);
        else if (input[i] == '$')
            add_node(head, expand_variable(input, &i, id), &ctl);
        else
            add_node(head, extract_regular_token(input, &i), &ctl);
    }
}


t_tokens	*check_parser(char *input, t_minishell *id)
{
    t_tokens *head = NULL;
    tokenize_input(input, &head, id);
    return head;
}
