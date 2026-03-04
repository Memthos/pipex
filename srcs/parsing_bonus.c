/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 19:31:42 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 19:35:50 by mperrine         ###   ########.fr       */
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
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_APPEND
				| O_CREAT, 0644);
	}
	else
	{
		pipex->fd_in = open(argv[0], O_RDONLY);
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_TRUNC
				| O_CREAT, 0644);
	}
}

t_pipex	pipex_base(int argc, char **argv, char **envp)
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
	if (pipe(pipex.pip_1) == -1)
		error(1, "Pipe failed to create", &pipex);
	return (pipex);
}
