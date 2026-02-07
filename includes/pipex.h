/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:39 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/07 20:06:46 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# include "../libs/libft_tools/headers/libft.h"

typedef struct s_pipex
{
	char	**env;
	char	**paths;
	char	*pwd;
	char	**cmd_in;
	char	**cmd_out;
	int		pipe[2];
	int		fd_in;
	int		fd_out;
}	t_pipex;

void	error(int code, char *msg, t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	close_fds(t_pipex *pipex);

#endif
