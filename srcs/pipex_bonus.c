/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 18:40:12 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static	int	parse_cmds(t_pipex *pipex, int argc, char **argv)
{
	size_t	i;

	if (pipex->limiter)
	{
		argc--;
		argv++;
	}
	pipex->nb_cmds = argc;
	pipex->cmds = malloc(sizeof(char **) * pipex->nb_cmds);
	if (!pipex->cmds)
		return (1);
	i = 0;
	while (i < pipex->nb_cmds)
	{
		pipex->cmds[i] = ft_split(argv[i], ' ');
		i++;
	}
	return (0);
}

static void	init_fds(t_pipex *pipex, int argc, char **argv)
{
	if (pipex->limiter)
	{
		pipex->fd_in = 0;
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	}
	else
	{
		pipex->fd_in = open(argv[0], O_RDONLY);
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	}
}

static t_pipex	pipex_base(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	size_t	i;

	pipex = (t_pipex){NULL, envp, NULL, NULL, NULL,
		0, 0, {-1, -1}, {-1, -1}, -1, -1};
	if (ft_strcmp(argv[0], "here_doc") == 0)
	{
		pipex.limiter = ft_strjoin(argv[1], "\n");
		if (!pipex.limiter)
			error(1, "Malloc error", &pipex);
	}
	init_fds(&pipex, argc, argv);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		pipex.paths = ft_split(envp[i] + 5, ':');
	if (parse_cmds(&pipex, argc - 2, argv + 1))
		error(1, "Malloc error", &pipex);
	pipex.pids = ft_calloc(pipex.nb_cmds, sizeof(pid_t));
	if (!pipex.pids)
		error(1, "Malloc error", &pipex);
	if (pipe(pipex.pipe_01) == -1)
		error(1, "Pipe failed to create", &pipex);
	return (pipex);
}

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
	size_t	i;

	if (argc < 5)
		error(2, "Wrong number of arguments", NULL);
	pipex = pipex_base(argc - 1, argv + 1, envp);
	if (pipex.limiter)
		here_doc(&pipex);
	status = exec_cmds(&pipex);
	close_fds(&pipex);
	i = 0;
	while (i < pipex.cur_cmd - 1)
		waitpid(pipex.pids[i++], NULL, 0);
	if (status)
	{
		waitpid(pipex.pids[i], NULL, 0);
		return (1);
	}
	else
		waitpid(pipex.pids[i], &status, 0);
	free_pipex(&pipex);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (0);
}
