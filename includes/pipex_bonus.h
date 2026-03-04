/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:39 by mperrine          #+#    #+#             */
/*   Updated: 2026/03/04 17:51:11 by mperrine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# include "../libs/libft_tools/includes/libft.h"

typedef struct s_pipex
{
	char	*limiter;
	char	**env;
	char	**paths;
	char	***cmds;
	pid_t	*pids;
	size_t	nb_cmds;
	size_t	cur_cmd;
	int		pipe_01[2];
	int		pipe_02[2];
	int		fd_in;
	int		fd_out;
}	t_pipex;

int		exec_cmds(t_pipex *pipex);
void	error(int code, char *msg, t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	close_fds(t_pipex *pipex);
void	close_fd(int *fd);
int		reset_pipe(int (*pipe_fds)[2]);

#endif
