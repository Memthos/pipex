/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/04 16:42:57 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pipex.h"

void	error(int code, char *msg, t_pipex *pipex)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		perror("Error:");
	exit(code);
}

t_pipex	pipex_base(char **argv, char **envp)
{
	t_pipex	pipex;

	pipex.fd_in = open(argv[1], O_RDONLY);
	pipex.fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	pipex.cmd_in = ft_split(argv[2], ' ');
	pipex.cmd_out = ft_split(argv[3], ' ');
	while (ft_strncmp(*envp, "PATH=", 5))
		envp++;
	pipex.env = ft_split(*envp + 5, ':');
	if (pipe(pipex.pipe) == -1)
		error(1, "Pipe failed to create", &pipex);
	return (pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		error(2, "Wrong number of arguments", 0);
	pipex = pipex_base(argv, envp);
}
