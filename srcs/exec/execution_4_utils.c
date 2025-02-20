/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_4_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: usoontra <usoontra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:44:50 by usoontra          #+#    #+#             */
/*   Updated: 2025/02/17 22:14:50 by usoontra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_cmd(t_iscmd *list, int limit)
{
	t_iscmd	*temp;

	temp = list;
	while (temp)
	{
		if(temp->index == limit)
			return (temp->cmd);
		temp = temp->next;
	}
	return (NULL);
}

char	*ft_strjoin3_agr(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*res;

	temp = ft_strjoin(s1 ,s2);
	if (!temp)
		ft_put_error(WARNING_MEM, "strjoin: ", EXIT_FAILURE, W_TITLE);
	res = ft_strjoin(temp, s3);
	free(temp);
	if (!res)
		ft_put_error(WARNING_MEM, "strjoin: ", EXIT_FAILURE, W_TITLE);
	return (res);
}

void	ft_print_cmd(t_iscmd *cmd, char *str)
{
	t_iscmd	*temp;
	int		i;

	temp = cmd;
	printf("===========	%s: print cmd		===========\n", str);
	while (temp)
	{
		i = 0;
		printf("======	cmd index %d	===========================\n", temp->index);
		while (temp->cmd[i])
		{
			printf("cmd = %s\n", temp->cmd[i]);
			i++;
		}
		temp = temp->next;
	}
	printf("====================================================\n");
}

void	ft_free_split(char **cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
	cmd = NULL;
}

void	ft_free_exec(t_tokens *list, t_minishell *id, t_fds *fd)
{
	t_iscmd	*temp;

	temp = id->cmd;
	while (temp)
	{
		ft_free_split(temp->cmd);
		temp = temp->next;
	}
	ft_free_list(id->cmd);
	if (fd)
	{
		ft_free_list(fd->in);
		ft_free_list(fd->out);
		free(fd);
	}
	ft_free_list(list);
}
