/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 10:24:00 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

static void	child_02(t_pipex *pipex)
{
	if (pipex->fd_out == -1)
		error(1, "no such file or directory", pipex);
	if (!pipex->cmd_out)
		error(1, "permission denied", pipex);
	if (dup2(pipex->pipe[0], 0) == -1)
		error(1, "Dup input file to standard input failed", pipex);
	if (dup2(pipex->fd_out, 1) == -1)
		error(1, "Dup pipe output to standard output failed", pipex);
	close_fds(pipex);
	execute(pipex, pipex->cmd_out);
}

static void	child_01(t_pipex *pipex)
{
	if (pipex->fd_in == -1)
		error (1, "no such file or directory", pipex);
	if (!pipex->cmd_in)
		error(1, "permission denied", pipex);
	if (dup2(pipex->fd_in, 0) == -1)
		error(1, "Dup input file to standard input failed", pipex);
	if (dup2(pipex->pipe[1], 1) == -1)
		error(1, "Dup pipe output to standard output failed", pipex);
	close_fds(pipex);
	execute(pipex, pipex->cmd_in);
}

static t_pipex	pipex_base(char **argv, char **envp)
{
	t_pipex	pipex;
	size_t	i;

	pipex = (t_pipex){envp, NULL, NULL, NULL, {-1, -1}, -1, -1};
	pipex.fd_in = open(argv[1], O_RDONLY);
	pipex.fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	pipex.cmd_in = ft_split(argv[2], ' ');
	pipex.cmd_out = ft_split(argv[3], ' ');
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp[i])
		pipex.paths = ft_split(envp[i] + 5, ':');
	if (pipe(pipex.pipe) == -1)
		error(1, "Pipe failed to create", &pipex);
	return (pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	childs[2];
	int		status;

	if (argc != 5)
		error(2, "Wrong number of arguments", NULL);
	pipex = pipex_base(argv, envp);
	childs[0] = fork();
	if (childs[0] == -1)
		error(1, "Child 01 creation failed", &pipex);
	if (childs[0] == 0)
		child_01(&pipex);
	childs[1] = fork();
	if (childs[1] == -1)
		error(1, "Child 02 creation failed", &pipex);
	if (childs[1] == 0)
		child_02(&pipex);
	free_pipex(&pipex);
	waitpid(childs[0], NULL, 0);
	waitpid(childs[1], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (0);
}
