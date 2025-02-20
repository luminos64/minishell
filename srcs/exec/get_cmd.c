/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 20:37:51 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/19 00:54:30 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_cmd_len(t_tokens *list)
{
	t_tokens	*temp;
	int			len;

	temp = list;
	len = 0;
	while (temp)
	{
		if (temp->status == PIPE)
			break ;
		if (temp->status == CMD || temp->status == CMD_ELEMENT)
			len++;
		temp = temp->next;
	}
	return (len);
}

static char	*add_cmd_3_sub_array(char *str)
{
	char	*temp;
	int		len;
	int		i;

	temp = NULL;
	len = ft_strlen(str);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (NULL);
	i = 0;
	while (len)
	{
		temp[i] = str[i];
		i++;
		len--;
	}
	temp[i] = 0;
	return (temp);
}

static char	**add_cmd_2(t_tokens *list, int len, t_tokens *temp, char **res)
{
	int			i;

	temp = list;
	res = malloc(sizeof(char *) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (temp)
	{
		if (temp->status == PIPE)
			break ;
		if (temp->status == CMD || temp->status == CMD_ELEMENT)
		{
			res[i] = add_cmd_3_sub_array(temp->str);
			if (!res[i])
			{
				ft_free_split(res);
				return (NULL);
			}
			i++;
		}
		temp = temp->next;
	}
	res[i] = NULL;
	return (res);
}

static int	add_cmd_1(t_iscmd **cmd, int pipe_no, t_tokens *list)
{
	t_tokens	*cmd_temp;
	t_iscmd		*temp;
	char		**res;
	int			len;

	cmd_temp = NULL;
	res = NULL;
	temp = NULL;
	temp = malloc(sizeof(t_iscmd));
	if (!temp)
		return (EXIT_FAILURE);
	len = ft_cmd_len(list);
	temp->index = pipe_no;
	temp->next = NULL;
	temp->cmd = add_cmd_2(list, len, cmd_temp, res);
	if (!temp->cmd)
	{
		free(temp);
		return (EXIT_FAILURE);
	}
	ft_list_addback((void **)cmd, temp);
	return (EXIT_SUCCESS);
}

int	ft_getcmd(t_iscmd **cmd, t_tokens *list)
{
	t_tokens	*temp;
	int			pipe_no;

	*cmd = NULL;
	temp = list;
	pipe_no = 1;
	while (temp)
	{
		if (temp->status == PIPE)
			pipe_no++;
		if (temp->status == CMD)
		{
			if (add_cmd_1(cmd, pipe_no, temp))
			{
				ft_put_error(WARNING_MEM, "cmd: ", NOT_EXIT, W_TITLE);
				return (EXIT_FAILURE);
			}
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}
