/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/09 00:58:45 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static void	execute(t_pipex *pipex, char **cmd)
{
	char	*tmp;
	size_t	i;

	tmp = NULL;
	if (cmd[0][0] == '.')
		tmp = ft_strjoin(pipex->pwd, &cmd[0][1]);
	else if (access(cmd[0], F_OK | X_OK) == -1)
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

static void	child(t_pipex *pipex, char *arg)
{
	char	**cmd;

	cmd = NULL;
	if (pipex->fd_in == -1)
		error (1, "no such file or directory", pipex);
	pipex->cmd = ft_split(arg, ' ');
	if (!pipex->cmd)
		error(1, "permission denied", pipex);
	if (dup2(pipex->fd_in, 0) == -1)
		error(1, "Dup input file to standard input failed", pipex);
	if (dup2(pipex->pipe[1], 1) == -1)
		error(1, "Dup pipe output to standard output failed", pipex);
	close_fds(pipex);
	if (!pipex->env)
		error(1, "Environment variable fail", pipex);
	execute(pipex, cmd);
}

static int	create_childs(t_pipex *pipex, int nb_cmds, char **argv)
{
	int		status;
	pid_t	pids[nb_cmds];
	size_t	i;

	i = 0;
	while (i < nb_cmds)
	{
		if (pipe(pipex->pipe) == -1)
			error(1, "Pipe failed to create", &pipex);
		pids[i] = fork();
		if (pids[i] == -1)
			error(1, "Child creation failed", &pipex);
		if (pids[i] == 0)
			child(&pipex, argv[i]);
		i++;
	}
	i = 0;
	while (i < nb_cmds - 1)
		waitpid(pids[i++], NULL, 0);
	waitpid(pids[i], status, 0);
	return (status);
}

static void	here_doc(t_pipex *pipex, int argc, char **argv)
{
	if (ft_strcmp(argv[1], "here_doc") == 0)
		pipex->here_doc = argv[2];
	if (pipex->here_doc)
	{
		pipex->fd_in = 0;
		pipex->fd_out = open(argv[argc - 1],
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	}
	else
	{
		pipex->fd_in = open(argv[1], O_RDONLY);
		pipex->fd_out = open(argv[argc - 1],
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
	}
}

static t_pipex	pipex_base(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	size_t	i;

	pipex = (t_pipex){.here_doc = NULL, .env = envp, .paths = NULL,
		.pwd = NULL, .cmd = NULL, .pipe = {-1, -1}, .fd_in = -1, .fd_out = -1};
	here_doc(&pipex, argc, argv);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		pipex.paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PWD=", 4))
		i++;
	if (envp[i])
		pipex.pwd = envp[i] + 4;
	return (pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		nb_cmds;
	int		status;

	if (argc < 5)
		error(2, "Wrong number of arguments", NULL);
	pipex = pipex_base(argc, argv, envp);
	nb_cmds = argc - 3;
	if (pipex.here_doc)
	{
		argv++;
		nb_cmds--;
	}
	status = create_childs(&pipex, nb_cmds, argv + 2);
	free_pipex(&pipex);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (0);
}
