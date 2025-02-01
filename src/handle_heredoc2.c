/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drenquin <drenquin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:26:11 by drenquin          #+#    #+#             */
/*   Updated: 2025/02/01 10:36:37 by drenquin         ###   ########.fr       */
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

char	*concat_strings(char *s1, char *s2)
{
	char	*result;
	char	*temp;

	if (!s1)
		return (ft_strdup(s2));
	temp = s1;
	result = ft_strjoin(s1, s2);
	free(temp);
	return (result);
}

char	*create_string_from_array(char **messages)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	if (!messages)
		return (NULL);
	while (messages[i])
	{
		result = concat_strings(result, messages[i]);
		if (!result)
			return (NULL);
		i++;
	}
	messages[i] = NULL;
	return (result);
}

void	free_tokens(char **tokens)
{
	int	j;

	if (tokens)
	{
		j = 0;
		while (tokens[j] != NULL)
		{
			free(tokens[j]);
			j++;
		}
		free(tokens);
	}
}

char	*process_line(char *input, t_context *ctx)

{
	char	**tokens;
	int		i;
	char	*result;

	tokens = tokenize_input_test(input);
	free(input);
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0' && tokens[i][1] != '$')
		{
			char *variable = get_node(&ctx->shell->env, tokens[i] + 1);
			free(tokens[i]);
			tokens[i] = NULL;
			if (variable != NULL)
				tokens[i] = ft_strdup(variable);
			else
				tokens[i] = ft_strdup("");
		}
		i++;
	}
	result = create_string_from_array(tokens);
	free_tokens(tokens);
	return (result);
}

/*char	*process_line(char *input, t_context *ctx)
{
	char	**tokens;
	int		i;
	int		index;
	char	*result;

	tokens = tokenize_input_test(input);
	free(input);
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i] != NULL)
	{
		if (tokens[i][0] == '$' && tokens[i][1] != '\0' && tokens[i][1] != '$' &&
				 get_node(&ctx->shell->env, tokens[i] + 1) != NULL)
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
}*/


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

void	write_messages(t_heredoc *data, char **messages)
{
	int	i;

	i = 0;
	while (messages[i])
	{
		write(data->pipe_fd[1], messages[i], strlen(messages[i]));
		i++;
	}
	free_message_array(messages);
	close(data->pipe_fd[1]);
}

void	write_and_wait(t_heredoc *data, t_shell *shell)
{
	char	*delimiter;
	char	**messages;
	t_list	*process;
	t_list	*check_last;

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
	write_messages(data, messages);
	ft_lstclear(&process, free);
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
