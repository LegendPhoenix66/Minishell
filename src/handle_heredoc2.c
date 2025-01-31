/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhopp <lhopp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:26:11 by drenquin          #+#    #+#             */
/*   Updated: 2025/01/30 13:05:23 by lhopp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_context	*init_context(t_shell *shell);
void		free_context(t_context *ctx);

t_heredoc	*init_pipe_data(void)
{
	t_heredoc	*data;

	data = malloc(sizeof(t_heredoc));
	if (!data)
		return (NULL);
	if (pipe(data->pipe_fd) == -1)
	{
		free(data);
		return (NULL);
	}
	return (data);
}

int	start_process(t_heredoc *data, t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	data->process_pid = fork();
	if (data->process_pid == -1)
	{
		perror("fork failed");
		return (1);
	}
	if (data->process_pid == 0)
	{
		close(data->pipe_fd[1]);
		dup2(data->pipe_fd[0], STDIN_FILENO);
		close(data->pipe_fd[0]);
		execute_command(cmd, shell);
		perror("process failed");
		exit(1);
	}
	else
		close(data->pipe_fd[0]);
	return (0);
}

void	free_message_array(char **messages)
{
	int	i;

	if (!messages)
		return ;
	i = 0;
	while (messages[i])
	{
		free(messages[i]);
		i++;
	}
	free(messages);
	messages = NULL;
}

char	*create_string_from_array(char **messages)
{
	char	*result;
	char	*temp;
	int		i;

	result = NULL;
	temp = NULL;
	i = 0;
	if (!messages)
		return (NULL);
	while (messages[i])
	{
		if (!result)
		{
			result = ft_strdup(messages[i]);
			if (!result)
				return (NULL);
		}
		else
		{
			temp = result;
			result = ft_strjoin(result, messages[i]);
			free(temp);
			if (!result)
				return (NULL);
		}
		i++;
	}
	messages[i] = NULL;
	return (result);
}

void	free_tokens(char **tokens)
{
	if (tokens)
	{
		for (int j = 0; tokens[j] != NULL; j++)
		{
			free(tokens[j]);
		}
		free(tokens);
	}
}

char	*process_line(char *input, t_context *ctx)
{
	char	**tokens;
	int		i;
	int		index;
	char	*result;

	tokens = tokenize_input_test(input);
	free(input);
	if (!tokens)
	{
		return (NULL);
	}
	i = 0;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0' && tokens[i][1] != '$')
		{
			index = find_index(tokens[i]);
			process_variable_substitution(tokens[i], &index, ctx);
			free(tokens[i]);
			tokens[i] = *ctx->new_content;
		}
		i++;
	}
	result = create_string_from_array(tokens);
	free_tokens(tokens);
	return (result);
}

char	**create_message_array(char *delimiter, t_shell *shell)
{
	char		**messages;
	int			capacity;
	int			size;
	t_context	*ctx;
	char		*input;
	char		*processed_input;
	char		**temp;

	messages = NULL;
	capacity = 10;
	size = 0;
	ctx = init_context(shell);
	if (!ctx)
	{
		return (NULL);
	}
	messages = malloc(sizeof(char *) * (capacity + 1));
	if (!messages)
	{
		free_context(ctx);
		return (NULL);
	}
	messages[0] = NULL;
	while (1)
	{
		input = get_input1();
		if (!input)
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		processed_input = process_line(input, ctx);
		if (!processed_input)
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		if (size >= capacity)
		{
			capacity *= 2;
			temp = realloc(messages, sizeof(char *) * (capacity + 1));
			if (!temp)
			{
				free(processed_input);
				free_message_array(messages);
				free_context(ctx);
				return (NULL);
			}
			messages = temp;
		}
		messages[size] = ft_strjoin(processed_input, "\n");
		free(processed_input);
		if (!messages[size])
		{
			free_message_array(messages);
			free_context(ctx);
			return (NULL);
		}
		size++;
		messages[size] = NULL;
	}
	free_context(ctx);
	return (messages);
}

t_context	*init_context(t_shell *shell)
{
	t_context	*ctx;

	ctx = (t_context *)malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	ctx->new_content = (char **)malloc(sizeof(char *));
	if (!ctx->new_content)
	{
		free(ctx);
		return (NULL);
	}
	*(ctx->new_content) = NULL;
	ctx->output_index = (int *)malloc(sizeof(int));
	if (!ctx->output_index)
	{
		free(ctx->new_content);
		free(ctx);
		return (NULL);
	}
	*(ctx->output_index) = 0;
	ctx->shell = shell;
	return (ctx);
}

void	free_context(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->new_content)
		free(ctx->new_content);
	if (ctx->output_index)
		free(ctx->output_index);
	free(ctx);
}

void	write_and_wait(t_heredoc *data, t_shell *shell)
{
	char	*delimiter;
	char	**messages;
	t_list	*process;
	t_list	*check_last;
	int		i;

	i = 0;
	check_last = last_token(&shell->tokens);
	if (ft_strcmp(check_last->content, "<<") == 0)
	{
		perror("bash: syntax error near unexpected token newline\n");
		shell->last_status = 2;
		return ;
	}
	process = cmd_after_heredoc(&shell->tokens);
	delimiter = process->content;
	messages = create_message_array(delimiter, shell);
	if (messages == NULL)
		return ;
	while (messages[i])
	{
		write(data->pipe_fd[1], messages[i], strlen(messages[i]));
		i++;
	}
	free_message_array(messages);
	ft_lstclear(&process, free);
	close(data->pipe_fd[1]);
	waitpid(data->process_pid, NULL, 0);
}

void	cleanup(t_heredoc *data)
{
	if (data)
	{
		close(data->pipe_fd[0]);
		close(data->pipe_fd[1]);
		free(data);
	}
}

void	write_nothing(t_shell *shell)
{
	char	*delimiter;
	t_list	*process;
	char	*input;

	process = cmd_after_heredoc(&shell->tokens);
	delimiter = process->content;
	while (1)
	{
		input = get_input1();
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	ft_lstclear(&process, free);
}

void	execute_heredoc(t_heredoc *data, t_cmd *cmd, t_shell *shell)
{
	int	process;

	data = init_pipe_data();
	if (!data)
	{
		perror("failled initialization\n");
		return ;
	}
	process = start_process(data, cmd, shell);
	if (process == 1)
	{
		write_nothing(shell);
		cleanup(data);
		return ;
	}
	if (process == 0)
	{
		write_and_wait(data, shell);
		cleanup(data);
		return ;
	}
}
