/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:36:57 by euniceleow        #+#    #+#             */
/*   Updated: 2025/02/20 20:59:20 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char *extract_quoted_string(const char *input, int *index)
// {
// 	char	quote;
// 	int		len;
// 	int		start;
// 	char	*temp;

// 	quote = input[*index];
// 	(*index)++;
// 	start = *index;
// 	while (input[*index] && input[*index] != quote)
// 		(*index)++;
// 	len = *index - start;
// 	temp = ft_strncpy(&input[start], len);
// 	if (input[*index] == quote)
// 		(*index)++;
// 	return (temp);
// }

char	*extract_quoted_string(const char *input, int *index)
{
	char	quote;
	int		len;
	int		start;
	int		last_quote_index;
	char	*temp;

	quote = input[*index];
	(*index)++;
	start = *index;
	last_quote_index = start;
	while (input[last_quote_index] != '\0')
	{
		if (input[last_quote_index] == quote)
			break;
		last_quote_index++;
	}
	if (input[last_quote_index] == quote)
	{
		len = last_quote_index - start;
		temp = ft_strncpy(&input[start], len);
		*index = last_quote_index + 1;
	}
	else
		temp = NULL;
	return temp;
}

char	*extract_operator(const char *input, int *index)
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

char	*extract_regular_token(const char *input, int *index)
{
	int		len;
	int		start;
	char	*temp;

	start = *index;
	while (input[*index] && !is_operator(input[*index]) && input[*index] != '$'
			&& input[*index] != ' ')
			(*index)++;
	len = *index - start;
	temp = ft_strncpy(&input[start], len);
	return (temp);
}

char	*expand_variable(const char *input, int *index, t_minishell *id)
{
	int		start;
	int		len;
	char	*temp;
	char	*value;

	(*index)++;
	start = *index;
	while (input[*index] && !is_operator(input[*index]) && input[*index] != ' ')
		(*index)++;
	len = *index - start;
	temp = ft_strncpy(&input[start], len);
	value = get_env_value(id, temp);
	if (value)
		return (value);
	else
		return ft_strdup("");
}
