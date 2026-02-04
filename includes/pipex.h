/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mperrine <mperrine@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:58:39 by mperrine          #+#    #+#             */
/*   Updated: 2026/02/04 15:10:14 by mperrine         ###   ########.fr       */
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
	char	**cmd;
	int		pipe[2];
	int		fd_in;
	int		fd_out;
}	t_pipex;


#endif
