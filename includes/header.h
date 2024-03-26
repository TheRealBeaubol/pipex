/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:12:33 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/26 15:21:50 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

/* *********************************** */
/* 			 Pipex Includes       	   */
/* *********************************** */

# include "libft.h"
# include <sys/wait.h>

/* *********************************** */
/* 			  Pipex Struct       	   */
/* *********************************** */

typedef struct s_data
{
	char	*file_1;
	char	*file_2;
	char	**cmd_1;
	char	**cmd_2;
	char	*path_cmd_1;
	char	*path_cmd_2;
	int		input_fd;
	int		output_fd;
	int		i;
	int		pipe_fd[2];
	int		here_doc;
	char	*cmd;
}	t_data;

/* *********************************** */
/* 			Mandatory functions   	   */
/* *********************************** */

char	**grep(char **env);
char	*cmd_path_init(char *cmd, char **path, t_data *data);
void	init(t_data *data, char **av, char **path);
void	putstr_and_exit(char *err_msg);

void	error(t_data data);
int		child_process(t_data data, int *pipe_fd, char **env);
int		parent_process(t_data data, int *pipe_fd, char **env);

/* *********************************** */
/* 			 Bonus functions    	   */
/* *********************************** */

void	heredoc(char **av, t_data *data);
int		process(char **env, t_data *data);
void	do_pipe(int ac, char **av, char **env, t_data *data);
void	pipex(int ac, char **av, char **env);

void	do_dup_and_execve(t_data *data, char **split_cmd, char **env);
void	open_files(int open_type, t_data *data, char **av, int ac);
void	get_stdin(char **av, int *pipe_fd);
char	*get_cmd(char **path, t_data *data);

#endif