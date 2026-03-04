/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:27:44 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 17:16:33 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static void	execute(t_pipex *pipex, char **cmd)
{
	char	*tmp;
	size_t	i;

	tmp = NULL;
	if (access(cmd[0], F_OK | X_OK) == -1)
	{
		i = 0;
		while (pipex->paths[i])
		{
			tmp = ft_strjoin_sep(pipex->paths[i++], cmd[0], '/');
			if (access(tmp, F_OK | X_OK) == 0)
				break ;
			free(tmp);
			tmp = NULL;
		}
	}
	if (tmp)
	{
		free(cmd[0]);
		cmd[0] = tmp;
	}
	execve(cmd[0], cmd, pipex->env);
	error(127, "Command not found", pipex);
}

static void	child(t_pipex *pipex, size_t idx, int fd_in, int fd_out)
{
	if (fd_in == -1 || fd_out == -1)
		error (1, "no such file or directory", pipex);
	if (!pipex->cmds[idx])
		error(1, "permission denied", pipex);
	if (dup2(fd_out, 1) == -1)
		error(1, "Dup failed", pipex);
	if (dup2(fd_in, 0) == -1)
		error(1, "Dup failed", pipex);
	close_fds(pipex);
	execute(pipex, pipex->cmds[idx]);
}

static int	cmds_loop(t_pipex *pipex)
{
	while (pipex->cur_cmd < pipex->nb_cmds - 1)
	{
		if (pipex->cur_cmd % 2)
		{
			if (reset_pipe(&pipex->pipe_02) == -1)
				return (1);
		}
		pipex->pids[pipex->cur_cmd] = fork();
		if (pipex->pids[pipex->cur_cmd] == -1)
			return (1);
		if (pipex->pids[pipex->cur_cmd] == 0)
		{
			if (pipex->cur_cmd % 2)
				child(pipex, pipex->cur_cmd, pipex->pipe_01[0], pipex->pipe_02[1]);
			else
				child(pipex, pipex->cur_cmd, pipex->pipe_02[0], pipex->pipe_01[1]);
		}
		if (pipex->cur_cmd % 2)
		{
			if (reset_pipe(&pipex->pipe_01) == -1)
				return (1);
		}
		pipex->cur_cmd++;
	}
	return (0);
}

int	exec_cmds(t_pipex *pipex)
{
	pipex->pids[0] = fork();
	if (pipex->pids[0] == -1)
		error(1, "Child 02 creation failed", pipex);
	if (pipex->pids[0] == 0)
		child(pipex, 0, pipex->fd_in, pipex->pipe_01[1]);
	pipex->cur_cmd++;
	if (cmds_loop(pipex))
		return (1);
	if (pipex->cur_cmd % 2)
		if (reset_pipe(&pipex->pipe_02) == -1)
			return (1);
	pipex->pids[pipex->cur_cmd] = fork();
	if (pipex->pids[pipex->cur_cmd] == -1)
		return (1);
	if (pipex->pids[pipex->cur_cmd] == 0)
	{
		if (pipex->cur_cmd % 2)
			child(pipex, pipex->cur_cmd, pipex->pipe_01[0], pipex->fd_out);
		else
			child(pipex, pipex->cur_cmd, pipex->pipe_02[0], pipex->fd_out);
	}
	pipex->cur_cmd++;
	return (0);
}
