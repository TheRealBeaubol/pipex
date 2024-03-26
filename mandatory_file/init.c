/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:53:32 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/26 15:08:18 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	putstr_and_exit(char *err_msg)
{
	ft_putstr_fd(err_msg, 2);
	exit (EXIT_FAILURE);
}

char	**grep(char **env)
{
	int		i;
	char	*cut_split;
	char	**splited_path;

	i = 0;
	if (!env || !*env)
		return (NULL);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env || !env[i])
		return (NULL);
	cut_split = ft_strcut(env[i], "PATH=");
	splited_path = ft_char_split(cut_split, ':');
	free(cut_split);
	return (splited_path);
}

char	*check_file_args(char *file)
{
	char	**split;

	split = ft_char_split(file, '.');
	if (!split || !split[1] || ft_strlen(split[1]) == 0)
		putstr_and_exit("File format error 🤓\n");
	ft_free_tab(split);
	return (file);
}

char	*cmd_path_init(char *cmd, char **path, t_data *data)
{
	char	*join_path;
	int		i;
	char	*path_cmd;

	i = -1;
	if (!path || !*path)
	{
		ft_free_tab(data->cmd_1);
		ft_free_tab(data->cmd_2);
		exit(1);
	}
	while (path[++i])
	{
		join_path = ft_strjoin(path[i], "/", NULL, 0b000);
		path_cmd = ft_strjoin(join_path, cmd, NULL, 0b000);
		free(join_path);
		if (access(path_cmd, 0) == 0)
			return (path_cmd);
		if (path[i + 1])
			free(path_cmd);
	}
	return (path_cmd);
}

void	init(t_data *data, char **av, char **path)
{
	data->file_1 = av[1];
	data->file_2 = av[4];
	data->cmd_1 = ft_char_split(av[2], ' ');
	data->cmd_2 = ft_char_split(av[3], ' ');
	data->path_cmd_1 = cmd_path_init(data->cmd_1[0], path, data);
	data->path_cmd_2 = cmd_path_init(data->cmd_2[0], path, data);
	ft_free_tab(path);
}
