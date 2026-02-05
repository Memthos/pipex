/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/05 13:35:17 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pipex.h"

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
	while (pipex->env[i++])
		free(pipex->env[i - 1]);
	if (pipex->env)
		free(pipex->env);
}

void	error(int code, char *msg, char *cmd, t_pipex *pipex)
{
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

	pipex = (t_pipex){.pipe = {-1, -1}, .fd_in = -1, .fd_out = -1};
	pipex.fd_in = open(argv[1], O_RDONLY);
	pipex.fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	pipex.cmd_in = ft_split(argv[2], ' ');
	pipex.cmd_out = ft_split(argv[3], ' ');
	while (envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	pipex.env = ft_split(*envp + 5, ':');
	if (pipe(pipex.pipe) == -1)
		error(1, "Pipe failed to create", &pipex, NULL);
	return (pipex);
}

void	hard_path_cmd(t_pipex *pipex)
{}

void	child_01(t_pipex *pipex)
{
	if (pipex->fd_in == -1)
		error (1, "no such file or directory", pipex->cmd_in[0], pipex);
	if (!pipex->cmd_in)
		error(1, "permission denied", NULL, pipex);
	if (!pipex->env)
		error(1, "Environment variable fail", NULL, pipex);
	if (dup2(pipex->fd_in, 0) == -1)
		error(1, "Dup input file to standard input failed", NULL, pipex);
	if (dup2(pipex->pipe[1], 1) == -1)
		error(1, "Dup pipe output to standard output failed", NULL, pipex);
	close_fds(pipex);
	if (pipex->cmd_in[0][0] == '.' || pipex->cmd_in[0][0] == '/')
		hard_path_cmd(pipex);

}

void	child_02(t_pipex *pipex)
{}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	pid_t	childs[2];
	int		status;

	if (argc != 5)
		error(2, "Wrong number of arguments", 0, NULL);
	pipex = pipex_base(argv, envp);
	childs[0] = fork();
	if (childs[0] == -1)
		error(1, "Child 01 creation failed", &pipex, NULL);
	if (childs[0] == 0)
		child_01(&pipex);
	childs[1] = fork();
	if (childs[1] == -1)
		error(1, "Child 02 creation failed", &pipex, NULL);
	if (childs[1] == 0)
		child_02(&pipex);
	waitpid(childs[0], NULL, 0);
	waitpid(childs[0], &status, 0);
}
