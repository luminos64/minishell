/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:14:19 by usoontra          #+#    #+#             */
/*   Updated: 2025/04/01 20:04:02 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_pipe(int *pipe, t_tokens *list)
{
	t_tokens	*temp;
	int			i;

	i = 1;
	temp = list;
	if (temp->status == PIPE)
		return (EXIT_FAILURE);
	while (temp)
	{
		if (temp->status == PIPE)
		{
			if (!temp->next || temp->next->status == PIPE)
				return (EXIT_FAILURE);
			i++;
		}
		temp = temp->next;
	}
	*pipe = i;
	return (EXIT_SUCCESS);
}

t_iscmd	*find_cmd(t_iscmd *list, int limit)
{
	t_iscmd	*temp;

	temp = list;
	while (temp)
	{
		if (temp->index == limit)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

char	*ft_strjoin3_a(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*res;

	temp = NULL;
	res = NULL;
	temp = ft_strjoin(s1, s2);
	if (s2)
		free(s2);
	if (!temp)
	{
		free(s3);
		ft_put_error(WARNING_MEM, "strjoin: ", EXIT_FAILURE, W_TITLE);
	}
	res = ft_strjoin(temp, s3);
	free(temp);
	free(s3);
	if (!res)
		ft_put_error(WARNING_MEM, "strjoin: ", EXIT_FAILURE, W_TITLE);
	return (res);
}

int	trans_tokens(t_minishell *id, t_redirect **fd, t_tokens *tokens)
{
	if (ft_getcmd(&id->cmd, tokens))
		return (EXIT_FAILURE);
	if (ft_redirect(fd, tokens))
		return (INVALID);
	if (ft_count_pipe(&id->pipe, tokens))
	{
		ft_put_error(WARNING_PIPE, NULL, NOT_EXIT, W_TITLE);
		return (INVALID);
	}
	return (EXIT_SUCCESS);
}
