/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_tools_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 19:33:57 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 18:39:44 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	reset_pipe(int (*pipe_fds)[2])
{
	int	ret;

	ret = 0;
	close_fd(&(*pipe_fds)[0]);
	close_fd(&(*pipe_fds)[1]);
	ret = pipe(*pipe_fds);
	return (ret);
}

void	close_fd(int *fd)
{
	if (*fd == -1)
		return ;
	close(*fd);
	*fd = -1;
}

void	close_fds(t_pipex *pipex)
{
	close_fd(&pipex->fd_in);
	close_fd(&pipex->fd_out);
	close_fd(&pipex->pipe_01[0]);
	close_fd(&pipex->pipe_01[1]);
	close_fd(&pipex->pipe_02[0]);
	close_fd(&pipex->pipe_02[1]);
}

static void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			free(tab[i++]);
		free(tab);
	}
}

void	free_pipex(t_pipex *pipex)
{
	size_t	i;

	close_fds(pipex);
	free_tab(pipex->paths);
	i = 0;
	if (pipex->cmds)
	{
		while (i < pipex->nb_cmds)
			free_tab(pipex->cmds[i++]);
		free(pipex->cmds);
	}
	if (pipex->pids)
		free(pipex->pids);
	if (pipex->limiter)
		free(pipex->limiter);
}

void	error(int code, char *msg, t_pipex *pipex)
{
	if (pipex)
		free_pipex(pipex);
	ft_putstr_fd("pipex: ", 2);
	ft_putendl_fd(msg, 2);
	exit(code);
}
