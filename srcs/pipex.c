/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:37 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/04 15:31:40 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pipex.h"

void	error(int code, char *msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		perror("Error:");
	exit(code);
}

void	pipex_base(char **argv, char **envp, t_pipex *pipex)
{
	if (pipe(pipex->pipe) == -1)
		error(1, "Pipe failed to create");
	while (ft_strncmp(*envp, "PATH=", 5))
		envp++;
	pipex->env = ft_split(*envp + 5, ':');
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		error(2, "Wrong number of arguments");
	pipex = NULL;
	pipex_base(argv, envp, &pipex);
}
