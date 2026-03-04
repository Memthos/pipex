/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 19:32:11 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static int	here_doc_data(t_pipex *pipex, char **buffer)
{
	char	*tmp;
	char	*read;

	read = NULL;
	while (1)
	{
		read = get_next_line(0);
		if (!read)
			continue ;
		if (ft_strcmp(read, pipex->limiter) == 0)
		{
			free(read);
			break ;
		}
		tmp = ft_strjoin(*buffer, read);
		free(*buffer);
		free(read);
		if (!tmp)
			return (1);
		*buffer = tmp;
	}
	return (0);
}

static void	here_doc(t_pipex *pipex)
{
	char	*data;
	int		pipe_fds[2];

	data = calloc(1, 1);
	if (!data)
		error(1, "Malloc error", pipex);
	data[0] = '\0';
	if (here_doc_data(pipex, &data))
		error(1, "Here_doc error", pipex);
	if (pipe(pipe_fds) == -1)
	{
		free(data);
		error(1, "Pipe failed to create", pipex);
	}
	write(pipe_fds[1], data, ft_strlen(data));
	free(data);
	close(pipe_fds[1]);
	pipex->fd_in = pipe_fds[0];
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		status;
	int		res;
	size_t	i;

	if (argc < 5)
		error(2, "Wrong number of arguments", NULL);
	pipex = pipex_base(argc - 1, argv + 1, envp);
	if (pipex.limiter)
		here_doc(&pipex);
	res = exec_cmds(&pipex);
	close_fds(&pipex);
	i = 0;
	while (i < pipex.cur_cmd - 1)
		waitpid(pipex.pids[i++], NULL, 0);
	waitpid(pipex.pids[i], &status, 0);
	free_pipex(&pipex);
	if (res)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (0);
}
