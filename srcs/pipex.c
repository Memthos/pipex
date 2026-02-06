/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/06 16:02:55 by mperrine         ###   ########.fr       */
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
	i = 0;
	while (pipex->paths[i++])
		free(pipex->paths[i - 1]);
	if (pipex->paths)
		free(pipex->paths);
	i = 0;
	while (pipex->cmd_in[i++])
		free(pipex->cmd_in[i - 1]);
	if (pipex->cmd_in)
		free(pipex->cmd_in);
	i = 0;
	while (pipex->cmd_out[i++])
		free(pipex->cmd_out[i - 1]);
	if (pipex->cmd_out)
		free(pipex->cmd_out);
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

t_pipex	pipex_base(char **argv, char **envp)
{
	t_pipex	pipex;
	char	*tmp;
	size_t	i;

	pipex = (t_pipex){.pipe = {-1, -1}, .fd_in = -1, .fd_out = -1, .env = envp};
	pipex.fd_in = open(argv[1], O_RDONLY);
	pipex.fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	pipex.cmd_in = ft_split(argv[2], ' ');
	pipex.cmd_out = ft_split(argv[3], ' ');
	i = 0;
	while (envp && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	pipex.paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (pipex.paths[i])
	{
		tmp = ft_strjoin(pipex.paths[i], "/");
		free(pipex.paths[i]);
		pipex.paths[i++] = tmp;
	}
	i = 0;
	while (envp && ft_strncmp(envp[i], "PWD=", 4))
		i++;
	pipex.pwd = envp[i] + 4;
	if (pipe(pipex.pipe) == -1)
		error(1, "Pipe failed to create", NULL, &pipex);
	return (pipex);
}

void	execute(t_pipex *pipex, char **cmd)
{
	char	*tmp;
	size_t	i;

	if (cmd[0][0] == '.')
	{
		tmp = ft_strjoin(pipex->pwd, &cmd[0][1]);
		free(cmd[0]);
		cmd[0] = tmp;
	}
	else if (access(cmd[0], F_OK | X_OK) == -1)
	{
		i = 0;
		while (pipex->paths[i])
		{
			tmp = ft_strjoin(pipex->paths[i++], cmd[0]);
			if (access(tmp, F_OK | X_OK) == 0)
			{
				free(cmd[0]);
				cmd[0] = tmp;
				break ;
			}
			free(tmp);
		}
	}
	execve(cmd[0], cmd, pipex->env);
	error(127, "Command not found", NULL, pipex);
}

void	child_01(t_pipex *pipex)
{
	if (pipex->fd_in == -1)
		error (1, "no such file or directory", pipex->cmd_in[0], pipex);
	if (!pipex->cmd_in)
		error(1, "permission denied", NULL, pipex);
	if (dup2(pipex->fd_in, 0) == -1)
		error(1, "Dup input file to standard input failed", NULL, pipex);
	if (dup2(pipex->pipe[1], 1) == -1)
		error(1, "Dup pipe output to standard output failed", NULL, pipex);
	close_fds(pipex);
	if (!pipex->paths)
		error(1, "Environment variable fail", NULL, pipex);
	execute(pipex, pipex->cmd_in);
}

void	child_02(t_pipex *pipex)
{
	if (pipex->fd_out == -1)
		error (1, "no such file or directory", pipex->cmd_out[0], pipex);
	if (!pipex->cmd_out)
		error(1, "permission denied", NULL, pipex);
	if (dup2(pipex->pipe[0], 0) == -1)
		error(1, "Dup input file to standard input failed", NULL, pipex);
	if (dup2(pipex->fd_out, 1) == -1)
		error(1, "Dup pipe output to standard output failed", NULL, pipex);
	close_fds(pipex);
	if (!pipex->paths)
		error(1, "Environment variable fail", NULL, pipex);
	execute(pipex, pipex->cmd_out);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	childs[2];
	int		status;

	if (argc != 5)
		error(2, "Wrong number of arguments", NULL, NULL);
	pipex = pipex_base(argv, envp);
	childs[0] = fork();
	if (childs[0] == -1)
		error(1, "Child 01 creation failed", NULL, &pipex);
	if (childs[0] == 0)
		child_01(&pipex);
	childs[1] = fork();
	if (childs[1] == -1)
		error(1, "Child 02 creation failed", NULL, &pipex);
	if (childs[1] == 0)
		child_02(&pipex);
	waitpid(childs[0], NULL, 0);
	waitpid(childs[1], &status, 0);
	free_pipex(&pipex);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
