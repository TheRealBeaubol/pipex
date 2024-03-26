/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 14:34:15 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/26 15:23:26 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	heredoc(char **av, t_data *data)
{
	int		pid;

	pipe(data->pipe_fd);
	pid = fork();
	if (pid == -1)
		putstr_and_exit("Error ඞ\n");
	if (pid == 0)
	{
		close(data->output_fd);
		get_stdin(av, data->pipe_fd);
	}
	else
	{
		close(data->pipe_fd[1]);
		dup2(data->pipe_fd[0], STDIN_FILENO);
		close(data->pipe_fd[0]);
		waitpid(pid, NULL, 0);
		dup2(data->output_fd, STDOUT_FILENO);
		close(data->output_fd);
	}
}

int	process(char **env, t_data *data)
{
	int		pid;
	char	**split_cmd;

	pipe(data->pipe_fd);
	pid = fork();
	split_cmd = ft_char_split(data->cmd, ' ');
	data->cmd = get_cmd(grep(env), data);
	if (pid == 0)
		do_dup_and_execve(data, split_cmd, env);
	else
	{
		close(data->pipe_fd[1]);
		dup2(data->pipe_fd[0], STDIN_FILENO);
		close(data->pipe_fd[0]);
		dup2(data->output_fd, STDOUT_FILENO);
		close(data->output_fd);
	}
	free(data->cmd);
	ft_free_tab(split_cmd);
	return (pid);
}

void	do_pipe(int ac, char **av, char **env, t_data *data)
{
	char	*path;
	char	**split_cmd;
	int		pid[1024];
	int		i;

	i = 0;
	while (data->i < ac - 2)
	{
		data->cmd = av[data->i++];
		pid[i++] = process(env, data);
	}
	data->cmd = av[ac - 2];
	path = get_cmd(grep(env), data);
	split_cmd = ft_char_split(av[ac - 2], ' ');
	if (execve(path, split_cmd, env) == -1)
	{
		free(path);
		ft_free_tab(split_cmd);
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (pid[++i])
		waitpid(pid[i], NULL, 0);
	free(path);
	ft_free_tab(split_cmd);
}

void	pipex(int ac, char **av, char **env)
{
	t_data	data;

	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		data.i = 3;
		if (ac < 6)
			putstr_and_exit("Not enough or too much arguments\n");
		open_files(0, &data, av, ac);
		heredoc(av, &data);
	}
	else
	{
		data.i = 2;
		open_files(1, &data, av, ac);
		dup2(data.input_fd, STDIN_FILENO);
		close(data.input_fd);
		if (data.i == -1)
		{
			close(data.output_fd);
			exit(1);
		}
	}
	do_pipe(ac, av, env, &data);
}
