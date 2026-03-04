/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 19:33:57 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 19:20:44 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	close_fd(int *fd)
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
	close_fd(&pipex->pipe[0]);
	close_fd(&pipex->pipe[1]);
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
	close_fds(pipex);
	free_tab(pipex->paths);
	free_tab(pipex->cmd_in);
	free_tab(pipex->cmd_out);
}

void	error(int code, char *msg, t_pipex *pipex)
{
	if (pipex)
		free_pipex(pipex);
	ft_putstr_fd("pipex: ", 2);
	ft_putendl_fd(msg, 2);
	exit(code);
}
