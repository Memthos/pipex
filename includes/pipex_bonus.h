/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:39 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/03 16:19:36 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# include "../libs/libft_tools/includes/libft.h"

typedef struct s_pipex
{
	char	*here_doc;
	char	**env;
	char	**paths;
	char	***cmds;
	int		pipe_01[2];
	int		pipe_02[2];
	int		fd_in;
	int		fd_out;
}	t_pipex;

void	error(int code, char *msg, t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	close_fds(t_pipex *pipex);

#endif
