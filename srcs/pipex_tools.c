/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 19:33:57 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/06 19:35:45 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_fds(t_pipex *pipex)
{
	if (pipex->fd_in != -1)
	{
		close(pipex->fd_in);
		pipex->fd_in = -1;
	}
	if (pipex->fd_out != -1)
	{
		close(pipex->fd_out);
		pipex->fd_out = -1;
	}
	if (pipex->pipe[0] != -1)
	{
		close(pipex->pipe[0]);
		pipex->pipe[0] = -1;
	}
	if (pipex->pipe[1] != -1)
	{
		close(pipex->pipe[1]);
		pipex->pipe[1] = -1;
	}
}

void	free_pipex(t_pipex *pipex)
{
	size_t	i;

	close_fds(pipex);
	if (pipex->paths)
	{
		i = 0;
		while (pipex->paths[i++])
			free(pipex->paths[i - 1]);
		free(pipex->paths);
	}
	if (pipex->cmd_in)
	{
		i = 0;
		while (pipex->cmd_in[i++])
			free(pipex->cmd_in[i - 1]);
		free(pipex->cmd_in);
	}
	if (pipex->cmd_out)
	{
		i = 0;
		while (pipex->cmd_out[i++])
			free(pipex->cmd_out[i - 1]);
		free(pipex->cmd_out);
	}
}

void	error(int code, char *msg, char *cmd, t_pipex *pipex)
{
	if (pipex)
		free_pipex(pipex);
	if (msg && cmd)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(" :", 2);
		ft_putendl_fd(cmd, 2);
	}
	else
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putendl_fd(msg, 2);
	}
	exit(code);
}
