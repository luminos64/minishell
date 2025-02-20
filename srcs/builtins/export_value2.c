/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euleow <euleow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:39:08 by euleow            #+#    #+#             */
/*   Updated: 2025/02/19 14:39:08 by euleow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_export_name(char *name, char *value)
{
    char    *temp;

    temp = name;
    if (temp[0] == '_' || ft_isalpha(temp[0]))
    {
        if(ft_strncmp((value), "", 2))
        {
            value = NULL;
        }
            return (1);
    }
    else
    {
        free(name);
        free(value);
        return (0);
    }
}

int	check_assign_value(char *name)
{
    int i;
    char *temp;

    i = 0;
    temp = name;
    while (temp[i])
    {
        if (temp[i] == '=')
            return (1);
        i++;
    }
    return (0);
}

void	strip_internal_quotes(char *value) 
{
	int	i;
    int	length;
    int	write_index;

	i = 0;
	write_index = 0;
	length = ft_strlen(value);
    if (!value) 
		return;
	while (i < length)
	{
        if (value[i] != '"') 
            value[write_index++] = value[i];
		i++;
    }
    value[write_index] = '\0';
}


int	parse_token(t_tokens *current, char **name, char **value)
{
    int j;
    int name_length;

    j = 0;
    *name = NULL;
    *value = NULL;
    while (current->str[j] && current->str[j] != '=')
        j++;
    name_length = j;
    if (current->str[j] == '=') 
        name_length += 1;
    *name = ft_substr(current->str, 0, name_length);
    if (current->str[j] == '=') 
	{
        j++;
        if (current->str[j] == '"' && current->str[strlen(current->str) - 1] == '"') 
		{ 
            *value = ft_substr(current->str, j + 1, strlen(current->str) - j - 2);
            strip_internal_quotes(*value);
        } 
		else 
            *value = ft_strdup(&current->str[j]);
        if (!check_export_name(*name, *value)) 
		{
            free(*name);
            free(*value);
            *name = NULL;
            *value = NULL;
            return (0);
        }
        return (1);
    }
    return(2);
}
