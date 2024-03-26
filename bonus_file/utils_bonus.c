/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:56:36 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/26 15:24:02 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	do_dup_and_execve(t_data *data, char **split_cmd, char **env)
{
	close(data->pipe_fd[0]);
	dup2(data->pipe_fd[1], STDOUT_FILENO);
	close(data->pipe_fd[1]);
	close(data->output_fd);
	if (execve(data->cmd, split_cmd, env) == -1)
	{
		ft_free_tab(split_cmd);
		free(data->cmd);
		exit(EXIT_FAILURE);
	}
}

void	open_files(int open_type, t_data *data, char **av, int ac)
{
	if (open_type == 0)
	{
		data->output_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (data->output_fd == -1)
			exit(EXIT_FAILURE);
	}
	else
	{
		data->input_fd = open(av[1], O_RDONLY);
		data->output_fd = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (data->output_fd == -1 || data->input_fd == -1)
			data->i = -1;
	}
}

char	**cmd_init(t_data *data, char **path)
{
	char	**tmp_split;

	if (!path || !*path)
	{
		close(data->output_fd);
		close(data->pipe_fd[0]);
		close(data->pipe_fd[1]);
		exit(1);
	}
	tmp_split = ft_char_split(data->cmd, ' ');
	data->cmd = tmp_split[0];
	return (tmp_split);
}

char	*get_cmd(char **path, t_data *data)
{
	char	*join_path;
	char	*path_cmd;
	char	**tmp_split;
	int		i;

	i = -1;
	tmp_split = cmd_init(data, path);
	while (path[++i])
	{
		join_path = ft_strjoin(path[i], "/", NULL, 0b000);
		path_cmd = ft_strjoin(join_path, data->cmd, NULL, 0b000);
		free(join_path);
		if (access(path_cmd, 0) == 0)
		{
			ft_free_tab(tmp_split);
			ft_free_tab(path);
			return (path_cmd);
		}
		if (path[i + 1])
			free(path_cmd);
	}
	ft_free_tab(tmp_split);
	ft_free_tab(path);
	return (path_cmd);
}

void	get_stdin(char **av, int *pipe_fd)
{
	char	*str;

	str = NULL;
	while (1)
	{
		if (ft_strncmp(str, av[2], ft_strlen(av[2])) == 0 \
		&& ft_strlen(av[2]) == ft_strlen(str) - 1)
		{
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			free(str);
			exit(EXIT_SUCCESS);
		}
		if (!str || str[ft_strlen(str) - !!ft_strlen(str)] == '\n')
		{
			ft_putstr_fd("here_doc> ", 2);
			ft_putstr_fd(str, pipe_fd[1]);
		}
		free(str);
		str = get_next_line(0, 0);
		if (!str)
			str = ft_strdup("");
	}
}
