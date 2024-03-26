/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 22:31:18 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/26 15:05:04 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	error(t_data data)
{
	ft_free_tab(data.cmd_1);
	ft_free_tab(data.cmd_2);
	free(data.path_cmd_1);
	free(data.path_cmd_2);
	exit(1);
}

int	child_process(t_data data, int *pipe_fd, char **env)
{
	int	fd;

	close(pipe_fd[0]);
	fd = open(data.file_1, O_RDONLY, 0777);
	if (fd == -1)
	{
		close(pipe_fd[1]);
		error(data);
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	close(pipe_fd[1]);
	if (execve(data.path_cmd_1, data.cmd_1, env) == -1)
		error(data);
	return (0);
}

int	parent_process(t_data data, int *pipe_fd, char **env)
{
	int	fd;

	close(pipe_fd[1]);
	fd = open(data.file_2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
	{
		close(pipe_fd[0]);
		error(data);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	close(pipe_fd[0]);
	if (execve(data.path_cmd_2, data.cmd_2, env) == -1)
		error(data);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int		pipe_fd[2];
	int		pid;
	t_data	data;

	if (ac != 5)
		putstr_and_exit("Not enough or too much arguments, \
make sure you have 4 arguments ! 🤓\n");
	else
	{
		init(&data, av, grep(env));
		if (pipe(pipe_fd) == -1)
			error(data);
		pid = fork();
		if (pid == -1)
			error(data);
		if (pid == 0)
			child_process(data, pipe_fd, env);
		parent_process(data, pipe_fd, env);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
